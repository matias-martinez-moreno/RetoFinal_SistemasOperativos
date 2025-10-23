#include "../include/args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Implementación propia de strdup para compatibilidad con C99
 */
char* mi_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* dup = malloc(len);
    if (dup) {
        strcpy(dup, s);
    }
    return dup;
}

/**
 * Parsea los argumentos de la línea de comandos
 */
Argumentos* parsear_argumentos(int argc, char* argv[]) {
    if (argc < 2) {
        mostrar_ayuda();
        return NULL;
    }
    
    // Asignar memoria para la estructura de argumentos
    Argumentos* args = malloc(sizeof(Argumentos));
    if (!args) {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        return NULL;
    }
    
    // Inicializar todos los campos
    args->comprimir = false;
    args->descomprimir = false;
    args->encriptar = false;
    args->desencriptar = false;
    args->algoritmo_comp = NULL;
    args->algoritmo_enc = NULL;
    args->archivo_entrada = NULL;
    args->archivo_salida = NULL;
    args->clave = NULL;
    
    // Parsear argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            args->comprimir = true;
        }
        else if (strcmp(argv[i], "-d") == 0) {
            args->descomprimir = true;
        }
        else if (strcmp(argv[i], "-e") == 0) {
            args->encriptar = true;
        }
        else if (strcmp(argv[i], "-u") == 0) {
            args->desencriptar = true;
        }
        else if (strcmp(argv[i], "--comp-alg") == 0) {
            if (i + 1 < argc) {
                args->algoritmo_comp = mi_strdup(argv[++i]);
            } else {
                fprintf(stderr, "Error: --comp-alg requiere un valor\n");
                liberar_argumentos(args);
                return NULL;
            }
        }
        else if (strcmp(argv[i], "--enc-alg") == 0) {
            if (i + 1 < argc) {
                args->algoritmo_enc = mi_strdup(argv[++i]);
            } else {
                fprintf(stderr, "Error: --enc-alg requiere un valor\n");
                liberar_argumentos(args);
                return NULL;
            }
        }
        else if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) {
                args->archivo_entrada = mi_strdup(argv[++i]);
            } else {
                fprintf(stderr, "Error: -i requiere un archivo de entrada\n");
                liberar_argumentos(args);
                return NULL;
            }
        }
        else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                args->archivo_salida = mi_strdup(argv[++i]);
            } else {
                fprintf(stderr, "Error: -o requiere un archivo de salida\n");
                liberar_argumentos(args);
                return NULL;
            }
        }
        else if (strcmp(argv[i], "-k") == 0) {
            if (i + 1 < argc) {
                args->clave = mi_strdup(argv[++i]);
            } else {
                fprintf(stderr, "Error: -k requiere una clave\n");
                liberar_argumentos(args);
                return NULL;
            }
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            mostrar_ayuda();
            liberar_argumentos(args);
            return NULL;
        }
        else {
            fprintf(stderr, "Error: Argumento desconocido: %s\n", argv[i]);
            liberar_argumentos(args);
            return NULL;
        }
    }
    
    // Validar argumentos requeridos
    if (!args->comprimir && !args->descomprimir && !args->encriptar && !args->desencriptar) {
        fprintf(stderr, "Error: Debe especificar una operación (-c, -d, -e, -u)\n");
        liberar_argumentos(args);
        return NULL;
    }
    
    if (!args->archivo_entrada) {
        fprintf(stderr, "Error: Debe especificar un archivo de entrada (-i)\n");
        liberar_argumentos(args);
        return NULL;
    }
    
    if (!args->archivo_salida) {
        fprintf(stderr, "Error: Debe especificar un archivo de salida (-o)\n");
        liberar_argumentos(args);
        return NULL;
    }
    
    // Validar algoritmos
    if ((args->comprimir || args->descomprimir) && !args->algoritmo_comp) {
        fprintf(stderr, "Error: Debe especificar un algoritmo de compresión (--comp-alg)\n");
        liberar_argumentos(args);
        return NULL;
    }
    
    if ((args->encriptar || args->desencriptar) && !args->algoritmo_enc) {
        fprintf(stderr, "Error: Debe especificar un algoritmo de encriptación (--enc-alg)\n");
        liberar_argumentos(args);
        return NULL;
    }
    
    return args;
}

/**
 * Libera la memoria asignada a los argumentos
 */
void liberar_argumentos(Argumentos* args) {
    if (!args) return;
    
    if (args->algoritmo_comp) free(args->algoritmo_comp);
    if (args->algoritmo_enc) free(args->algoritmo_enc);
    if (args->archivo_entrada) free(args->archivo_entrada);
    if (args->archivo_salida) free(args->archivo_salida);
    if (args->clave) free(args->clave);
    
    free(args);
}

/**
 * Imprime el uso del programa
 */
void mostrar_ayuda(void) {
    printf("GSEA - Utilidad de Gestión Segura y Eficiente de Archivos\n");
    printf("Uso: ./gsea [opciones]\n\n");
    printf("Operaciones:\n");
    printf("  -c                    Comprimir archivo\n");
    printf("  -d                    Descomprimir archivo\n");
    printf("  -e                    Encriptar archivo (no implementado)\n");
    printf("  -u                    Desencriptar archivo (no implementado)\n\n");
    printf("Opciones:\n");
    printf("  --comp-alg ALGORITMO  Algoritmo de compresión (rle)\n");
    printf("  --enc-alg ALGORITMO   Algoritmo de encriptación (no implementado)\n");
    printf("  -i ARCHIVO            Archivo de entrada\n");
    printf("  -o ARCHIVO            Archivo de salida\n");
    printf("  -k CLAVE              Clave para encriptación (no implementado)\n");
    printf("  -h, --help            Mostrar esta ayuda\n\n");
    printf("Ejemplos:\n");
    printf("  ./gsea -c --comp-alg rle -i archivo.txt -o archivo.txt.rle\n");
    printf("  ./gsea -d --comp-alg rle -i archivo.txt.rle -o archivo_descomprimido.txt\n");
}
