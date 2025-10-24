#include "../include/directory_processor.h"
#include "../include/file_manager.h"
#include "../include/compression.h"
#include "../include/encryption.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef DT_REG
#define DT_REG 8
#endif

// Implementación propia de strdup para compatibilidad con C99
char* mi_strdup_dir(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* dup = malloc(len);
    if (dup) {
        strcpy(dup, s);
    }
    return dup;
}

/**
 * Procesa un directorio completo aplicando la operación especificada
 * 
 * Esta función utiliza las llamadas al sistema opendir(), readdir() y closedir()
 * para procesar todos los archivos regulares dentro de un directorio.
 * Aplica la operación especificada (compresión, descompresión, encriptación, desencriptación)
 * a cada archivo encontrado.
 */
int procesar_directorio(const char* ruta_directorio, const char* ruta_salida,
                        char operacion, const char* algoritmo_comp, 
                        const char* algoritmo_enc, const char* clave) {
    if (!ruta_directorio || !ruta_salida) {
        fprintf(stderr, "Error: Parámetros inválidos para procesar_directorio\n");
        return -1;
    }
    
    // Verificar que el directorio de entrada existe
    if (!es_directorio(ruta_directorio)) {
        fprintf(stderr, "Error: '%s' no es un directorio válido\n", ruta_directorio);
        return -1;
    }
    
    // Crear el directorio de salida si no existe
    if (crear_directorio_salida(ruta_salida) != 0) {
        fprintf(stderr, "Error: No se pudo crear el directorio de salida '%s'\n", ruta_salida);
        return -1;
    }
    
    // Abrir el directorio usando llamada al sistema
    DIR* dir = opendir(ruta_directorio);
    if (!dir) {
        fprintf(stderr, "Error: No se pudo abrir el directorio '%s': %s\n", 
                ruta_directorio, strerror(errno));
        return -1;
    }
    
    printf("Procesando directorio: %s\n", ruta_directorio);
    printf("Directorio de salida: %s\n", ruta_salida);
    printf("Operación: %c\n", operacion);
    
    struct dirent* entrada;
    int archivos_procesados = 0;
    int archivos_exitosos = 0;
    
    // Procesar cada entrada del directorio
    while ((entrada = readdir(dir)) != NULL) {
        // Saltar entradas especiales
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }
        
        // Solo procesar archivos regulares
        if (entrada->d_type == DT_REG) {
            archivos_procesados++;
            
            // Construir rutas completas
            char ruta_archivo[PATH_MAX];
            char ruta_archivo_salida[PATH_MAX];
            
            snprintf(ruta_archivo, sizeof(ruta_archivo), "%s/%s", ruta_directorio, entrada->d_name);
            snprintf(ruta_archivo_salida, sizeof(ruta_archivo_salida), "%s/%s", ruta_salida, entrada->d_name);
            
            printf("Procesando archivo: %s\n", entrada->d_name);
            
            // Procesar el archivo según la operación
            if (procesar_archivo_individual(ruta_archivo, ruta_archivo_salida, 
                                          operacion, algoritmo_comp, algoritmo_enc, clave) == 0) {
                archivos_exitosos++;
                printf("Archivo procesado exitosamente: %s\n", entrada->d_name);
            } else {
                fprintf(stderr, "Error: No se pudo procesar el archivo '%s'\n", entrada->d_name);
            }
        }
    }
    
    // Cerrar el directorio
    closedir(dir);
    
    printf("Procesamiento completado: %d/%d archivos procesados exitosamente\n", 
           archivos_exitosos, archivos_procesados);
    
    return (archivos_exitosos == archivos_procesados) ? 0 : -1;
}

/**
 * Procesa un archivo individual aplicando la operación especificada
 * 
 * Esta función procesa un archivo individual aplicando la operación
 * de compresión, descompresión, encriptación o desencriptación.
 */
int procesar_archivo_individual(const char* archivo_entrada, const char* archivo_salida,
                               char operacion, const char* algoritmo_comp,
                               const char* algoritmo_enc, const char* clave) {
    // Leer el archivo de entrada
    char* contenido = NULL;
    size_t tamano = 0;
    
    if (leer_archivo(archivo_entrada, &contenido, &tamano) != 0) {
        return -1;
    }
    
    char* datos_procesados = NULL;
    size_t tamano_procesado = 0;
    int resultado = 0;
    
    // Aplicar la operación según el tipo
    switch (operacion) {
        case 'c': // Comprimir
            if (strcmp(algoritmo_comp, "rle") == 0) {
                resultado = comprimir_rle(contenido, tamano, &datos_procesados, &tamano_procesado);
            } else {
                fprintf(stderr, "Error: Algoritmo de compresión no soportado: %s\n", algoritmo_comp);
                resultado = -1;
            }
            break;
            
        case 'd': // Descomprimir
            if (strcmp(algoritmo_comp, "rle") == 0) {
                resultado = descomprimir_rle(contenido, tamano, &datos_procesados, &tamano_procesado);
            } else {
                fprintf(stderr, "Error: Algoritmo de compresión no soportado: %s\n", algoritmo_comp);
                resultado = -1;
            }
            break;
            
        case 'e': // Encriptar
            if (strcmp(algoritmo_enc, "vigenere") == 0) {
                if (!clave) {
                    fprintf(stderr, "Error: Se requiere una clave para encriptación\n");
                    resultado = -1;
                } else {
                    resultado = encriptar_vigenere(contenido, tamano, clave, &datos_procesados, &tamano_procesado);
                }
            } else {
                fprintf(stderr, "Error: Algoritmo de encriptación no soportado: %s\n", algoritmo_enc);
                resultado = -1;
            }
            break;
            
        case 'u': // Desencriptar
            if (strcmp(algoritmo_enc, "vigenere") == 0) {
                if (!clave) {
                    fprintf(stderr, "Error: Se requiere una clave para desencriptación\n");
                    resultado = -1;
                } else {
                    resultado = desencriptar_vigenere(contenido, tamano, clave, &datos_procesados, &tamano_procesado);
                }
            } else {
                fprintf(stderr, "Error: Algoritmo de encriptación no soportado: %s\n", algoritmo_enc);
                resultado = -1;
            }
            break;
            
        default:
            fprintf(stderr, "Error: Operación no válida: %c\n", operacion);
            resultado = -1;
            break;
    }
    
    // Escribir el archivo de salida si la operación fue exitosa
    if (resultado == 0 && datos_procesados) {
        resultado = escribir_archivo(archivo_salida, datos_procesados, tamano_procesado);
    }
    
    // Liberar memoria
    if (contenido) free(contenido);
    if (datos_procesados) {
        if (operacion == 'e' || operacion == 'u') {
            liberar_datos_encriptados(datos_procesados);
        } else {
            liberar_datos(datos_procesados);
        }
    }
    
    return resultado;
}

/**
 * Lista todos los archivos regulares en un directorio
 */
int listar_archivos_directorio(const char* ruta_directorio, char*** archivos, size_t* num_archivos) {
    if (!ruta_directorio || !archivos || !num_archivos) {
        return -1;
    }
    
    DIR* dir = opendir(ruta_directorio);
    if (!dir) {
        return -1;
    }
    
    // Contar archivos primero
    *num_archivos = 0;
    struct dirent* entrada;
    while ((entrada = readdir(dir)) != NULL) {
        if (entrada->d_type == DT_REG && strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
            (*num_archivos)++;
        }
    }
    
    // Asignar memoria para el array de archivos
    *archivos = malloc(*num_archivos * sizeof(char*));
    if (!*archivos) {
        closedir(dir);
        return -1;
    }
    
    // Volver al inicio del directorio
    rewinddir(dir);
    
    // Llenar el array con las rutas de los archivos
    size_t indice = 0;
    while ((entrada = readdir(dir)) != NULL) {
        if (entrada->d_type == DT_REG && strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
            char ruta_completa[PATH_MAX];
            snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta_directorio, entrada->d_name);
            (*archivos)[indice] = mi_strdup_dir(ruta_completa);
            indice++;
        }
    }
    
    closedir(dir);
    return 0;
}

/**
 * Crea un directorio de salida si no existe
 */
int crear_directorio_salida(const char* ruta_directorio) {
    if (!ruta_directorio) {
        return -1;
    }
    
    // Verificar si el directorio ya existe
    struct stat st;
    if (stat(ruta_directorio, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            return 0; // El directorio ya existe
        } else {
            return -1; // Existe pero no es un directorio
        }
    }
    
    // Crear el directorio con permisos 755
    if (mkdir(ruta_directorio, 0755) == 0) {
        printf("Directorio creado: %s\n", ruta_directorio);
        return 0;
    } else {
        fprintf(stderr, "Error: No se pudo crear el directorio '%s': %s\n", 
                ruta_directorio, strerror(errno));
        return -1;
    }
}

/**
 * Verifica si una ruta es un directorio
 */
int es_directorio(const char* ruta) {
    if (!ruta) {
        return -1;
    }
    
    struct stat st;
    if (stat(ruta, &st) == -1) {
        return -1;
    }
    
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

/**
 * Libera la memoria asignada para la lista de archivos
 */
void liberar_lista_archivos(char** archivos, size_t num_archivos) {
    if (!archivos) {
        return;
    }
    
    for (size_t i = 0; i < num_archivos; i++) {
        if (archivos[i]) {
            free(archivos[i]);
        }
    }
    
    free(archivos);
}
