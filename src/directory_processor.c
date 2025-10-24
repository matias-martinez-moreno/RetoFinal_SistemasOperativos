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
#include <pthread.h>
#include <unistd.h>

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

// Estructura para pasar datos a los hilos
typedef struct {
    char ruta_entrada[PATH_MAX];
    char ruta_salida[PATH_MAX];
    char operacion;
    const char* algoritmo_comp;
    const char* algoritmo_enc;
    const char* clave;
    int resultado;
} DatosHilo;

// Función que ejecuta cada hilo
void* procesar_archivo_hilo(void* arg) {
    DatosHilo* datos = (DatosHilo*)arg;
    
    printf("Hilo procesando: %s\n", datos->ruta_entrada);
    
    // Procesar el archivo individual
    datos->resultado = procesar_archivo_individual(
        datos->ruta_entrada, 
        datos->ruta_salida,
        datos->operacion,
        datos->algoritmo_comp,
        datos->algoritmo_enc,
        datos->clave
    );
    
    printf("Hilo completado: %s (resultado: %d)\n", datos->ruta_entrada, datos->resultado);
    return NULL;
}

/**
 * Procesa un directorio completo aplicando la operación especificada CON CONCURRENCIA
 * 
 * Esta función utiliza pthreads para procesar múltiples archivos en paralelo,
 * mejorando el rendimiento en sistemas multinúcleo.
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
    
    // Crear directorio de salida si no existe
    if (crear_directorio_salida(ruta_salida) != 0) {
        fprintf(stderr, "Error: No se pudo crear el directorio de salida '%s'\n", ruta_salida);
        return -1;
    }
    
    // Abrir directorio de entrada
    DIR* dir = opendir(ruta_directorio);
    if (!dir) {
        fprintf(stderr, "Error: No se pudo abrir el directorio '%s': %s\n", 
                ruta_directorio, strerror(errno));
        return -1;
    }
    
    // Contar archivos para crear hilos
    struct dirent* entrada;
    int num_archivos = 0;
    
    // Primera pasada: contar archivos
    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0 &&
            entrada->d_type == DT_REG) {
            num_archivos++;
        }
    }
    
    if (num_archivos == 0) {
        printf("No se encontraron archivos para procesar\n");
        closedir(dir);
        return 0;
    }
    
    printf("Procesando directorio con CONCURRENCIA: %s\n", ruta_directorio);
    printf("Archivos encontrados: %d\n", num_archivos);
    printf("Usando hilos para procesamiento paralelo\n");
    
    // Crear arrays para hilos y datos
    pthread_t* hilos = malloc(num_archivos * sizeof(pthread_t));
    DatosHilo* datos_hilos = malloc(num_archivos * sizeof(DatosHilo));
    
    if (!hilos || !datos_hilos) {
        fprintf(stderr, "Error: No se pudo asignar memoria para hilos\n");
        closedir(dir);
        free(hilos);
        free(datos_hilos);
        return -1;
    }
    
    // Segunda pasada: crear hilos para cada archivo
    rewinddir(dir);
    int indice = 0;
    
    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0 &&
            entrada->d_type == DT_REG) {
            
            // Configurar datos del hilo
            snprintf(datos_hilos[indice].ruta_entrada, sizeof(datos_hilos[indice].ruta_entrada),
                    "%s/%s", ruta_directorio, entrada->d_name);
            snprintf(datos_hilos[indice].ruta_salida, sizeof(datos_hilos[indice].ruta_salida),
                    "%s/%s", ruta_salida, entrada->d_name);
            datos_hilos[indice].operacion = operacion;
            datos_hilos[indice].algoritmo_comp = algoritmo_comp;
            datos_hilos[indice].algoritmo_enc = algoritmo_enc;
            datos_hilos[indice].clave = clave;
            datos_hilos[indice].resultado = -1;
            
            // Crear hilo
            if (pthread_create(&hilos[indice], NULL, procesar_archivo_hilo, &datos_hilos[indice]) != 0) {
                fprintf(stderr, "Error: No se pudo crear hilo para %s\n", entrada->d_name);
                datos_hilos[indice].resultado = -1;
            }
            
            indice++;
        }
    }
    
    closedir(dir);
    
    // Esperar a que todos los hilos terminen
    printf("Esperando a que terminen todos los hilos...\n");
    int archivos_procesados = 0;
    int errores = 0;
    
    for (int i = 0; i < num_archivos; i++) {
        pthread_join(hilos[i], NULL);
        
        if (datos_hilos[i].resultado == 0) {
            archivos_procesados++;
        } else {
            errores++;
        }
    }
    
    // Liberar memoria
    free(hilos);
    free(datos_hilos);
    
    printf("\nResumen del procesamiento CONCURRENTE:\n");
    printf("- Archivos procesados: %d\n", archivos_procesados);
    printf("- Errores: %d\n", errores);
    printf("- Hilos utilizados: %d\n", num_archivos);
    
    if (errores > 0) {
        printf("Advertencia: Se encontraron %d errores durante el procesamiento\n", errores);
        return -1;
    }
    
    return 0;
}

// Implementar operaciones combinadas
int procesar_operacion_combinada(const char* ruta_entrada, const char* ruta_salida,
                                 const char* operaciones, const char* algoritmo_comp,
                                 const char* algoritmo_enc, const char* clave) {
    if (!ruta_entrada || !ruta_salida || !operaciones) {
        fprintf(stderr, "Error: Parámetros inválidos para operación combinada\n");
        return -1;
    }
    
    printf("Procesando operación combinada: %s\n", operaciones);
    
    // -ce: comprimir y encriptar
    if (strcmp(operaciones, "-ce") == 0) {
        char ruta_intermedia[PATH_MAX];
        snprintf(ruta_intermedia, sizeof(ruta_intermedia), "%s.temp", ruta_salida);
        
        // Paso 1: Comprimir
        printf("Paso 1: Comprimiendo archivo...\n");
        if (procesar_archivo_individual(ruta_entrada, ruta_intermedia, 'c', 
                                      algoritmo_comp, NULL, NULL) != 0) {
            fprintf(stderr, "Error en compresión\n");
            return -1;
        }
        
        // Paso 2: Encriptar
        printf("Paso 2: Encriptando archivo comprimido...\n");
        if (procesar_archivo_individual(ruta_intermedia, ruta_salida, 'e',
                                      NULL, algoritmo_enc, clave) != 0) {
            fprintf(stderr, "Error en encriptación\n");
            unlink(ruta_intermedia); // Limpiar archivo temporal
            return -1;
        }
        
        // Limpiar archivo temporal
        unlink(ruta_intermedia);
        printf("Operación -ce completada exitosamente\n");
        return 0;
    }
    
    // -de: descomprimir y encriptar
    if (strcmp(operaciones, "-de") == 0) {
        char ruta_intermedia[PATH_MAX];
        snprintf(ruta_intermedia, sizeof(ruta_intermedia), "%s.temp", ruta_salida);
        
        // Paso 1: Descomprimir
        printf("Paso 1: Descomprimiendo archivo...\n");
        if (procesar_archivo_individual(ruta_entrada, ruta_intermedia, 'd',
                                      algoritmo_comp, NULL, NULL) != 0) {
            fprintf(stderr, "Error en descompresión\n");
            return -1;
        }
        
        // Paso 2: Encriptar
        printf("Paso 2: Encriptando archivo descomprimido...\n");
        if (procesar_archivo_individual(ruta_intermedia, ruta_salida, 'e',
                                      NULL, algoritmo_enc, clave) != 0) {
            fprintf(stderr, "Error en encriptación\n");
            unlink(ruta_intermedia);
            return -1;
        }
        
        unlink(ruta_intermedia);
        printf("Operación -de completada exitosamente\n");
        return 0;
    }
    
    // -ec: encriptar y comprimir
    if (strcmp(operaciones, "-ec") == 0) {
        char ruta_intermedia[PATH_MAX];
        snprintf(ruta_intermedia, sizeof(ruta_intermedia), "%s.temp", ruta_salida);
        
        // Paso 1: Encriptar
        printf("Paso 1: Encriptando archivo...\n");
        if (procesar_archivo_individual(ruta_entrada, ruta_intermedia, 'e',
                                      NULL, algoritmo_enc, clave) != 0) {
            fprintf(stderr, "Error en encriptación\n");
            return -1;
        }
        
        // Paso 2: Comprimir
        printf("Paso 2: Comprimiendo archivo encriptado...\n");
        if (procesar_archivo_individual(ruta_intermedia, ruta_salida, 'c',
                                      algoritmo_comp, NULL, NULL) != 0) {
            fprintf(stderr, "Error en compresión\n");
            unlink(ruta_intermedia);
            return -1;
        }
        
        unlink(ruta_intermedia);
        printf("Operación -ec completada exitosamente\n");
        return 0;
    }
    
    // -du: desencriptar y descomprimir
    if (strcmp(operaciones, "-du") == 0) {
        char ruta_intermedia[PATH_MAX];
        snprintf(ruta_intermedia, sizeof(ruta_intermedia), "%s.temp", ruta_salida);
        
        // Paso 1: Desencriptar
        printf("Paso 1: Desencriptando archivo...\n");
        if (procesar_archivo_individual(ruta_entrada, ruta_intermedia, 'u',
                                      NULL, algoritmo_enc, clave) != 0) {
            fprintf(stderr, "Error en desencriptación\n");
            return -1;
        }
        
        // Paso 2: Descomprimir
        printf("Paso 2: Descomprimiendo archivo desencriptado...\n");
        if (procesar_archivo_individual(ruta_intermedia, ruta_salida, 'd',
                                      algoritmo_comp, NULL, NULL) != 0) {
            fprintf(stderr, "Error en descompresión\n");
            unlink(ruta_intermedia);
            return -1;
        }
        
        unlink(ruta_intermedia);
        printf("Operación -du completada exitosamente\n");
        return 0;
    }
    
    fprintf(stderr, "Error: Operación combinada no soportada: %s\n", operaciones);
    return -1;
}

// Resto de funciones existentes...
int es_directorio(const char* ruta) {
    struct stat st;
    if (stat(ruta, &st) == -1) {
        return -1;
    }
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

int crear_directorio_salida(const char* ruta) {
    struct stat st;
    if (stat(ruta, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            return 0; // Ya existe
        } else {
            return -1; // Existe pero no es directorio
        }
    }
    
    if (mkdir(ruta, 0755) == 0) {
        printf("Directorio creado: %s\n", ruta);
        return 0;
    } else {
        fprintf(stderr, "Error: No se pudo crear el directorio '%s': %s\n", 
                ruta, strerror(errno));
        return -1;
    }
}

int procesar_archivo_individual(const char* ruta_entrada, const char* ruta_salida,
                               char operacion, const char* algoritmo_comp,
                               const char* algoritmo_enc, const char* clave) {
    // Leer archivo
    char* contenido;
    size_t tamano;
    
    if (leer_archivo(ruta_entrada, &contenido, &tamano) != 0) {
        return -1;
    }
    
    char* datos_procesados = NULL;
    size_t tamano_procesado = 0;
    int resultado = 0;
    
    // Aplicar operación
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
    }
    
    // Escribir resultado si fue exitoso
    if (resultado == 0 && datos_procesados) {
        if (escribir_archivo(ruta_salida, datos_procesados, tamano_procesado) != 0) {
            resultado = -1;
        }
        free(datos_procesados);
    }
    
    free(contenido);
    return resultado;
}

int listar_archivos_directorio(const char* ruta_directorio, char*** archivos, size_t* num_archivos) {
    DIR* dir = opendir(ruta_directorio);
    if (!dir) {
        return -1;
    }
    
    char** lista_archivos = malloc(100 * sizeof(char*));
    if (!lista_archivos) {
        closedir(dir);
        return -1;
    }
    
    size_t contador = 0;
    struct dirent* entrada;
    
    while ((entrada = readdir(dir)) != NULL && contador < 100) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0 &&
            entrada->d_type == DT_REG) {
            lista_archivos[contador] = mi_strdup_dir(entrada->d_name);
            contador++;
        }
    }
    
    closedir(dir);
    *archivos = lista_archivos;
    *num_archivos = contador;
    return 0;
}

void liberar_lista_archivos(char** archivos, size_t num_archivos) {
    if (!archivos) return;
    
    for (size_t i = 0; i < num_archivos; i++) {
        free(archivos[i]);
    }
    free(archivos);
}
