#include "../include/args.h"
#include "../include/file_manager.h"
#include "../include/compression.h"
#include "../include/encryption.h"
#include "../include/directory_processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Función principal del programa GSEA
 * 
 * GSEA es una utilidad de línea de comandos para comprimir/descomprimir
 * archivos usando el algoritmo RLE (Run-Length Encoding).
 */
int main(int argc, char* argv[]) {
    printf("GSEA - Utilidad de Gestión Segura y Eficiente de Archivos\n");
    printf("Versión: 2.1 (Fase 2 - Compresión RLE + Encriptación Vigenère + Directorios)\n\n");
    
    // Parsear argumentos de línea de comandos
    Argumentos* args = parsear_argumentos(argc, argv);
    if (!args) {
        return 1;
    }
    
    // Verificar que el archivo o directorio de entrada existe
    int existe = archivo_existe(args->archivo_entrada);
    if (existe == 0) {
        fprintf(stderr, "Error: El archivo o directorio de entrada '%s' no existe\n", args->archivo_entrada);
        liberar_argumentos(args);
        return 1;
    } else if (existe == -1) {
        fprintf(stderr, "Error: No se pudo acceder al archivo o directorio de entrada '%s'\n", args->archivo_entrada);
        liberar_argumentos(args);
        return 1;
    }
    
    // Verificar si la entrada es un directorio
    int es_dir = es_directorio(args->archivo_entrada);
    if (es_dir == 1) {
        // Procesar directorio completo
        printf("Procesando directorio: %s\n", args->archivo_entrada);
        
        char operacion = 'c'; // Por defecto comprimir
        if (args->comprimir) operacion = 'c';
        else if (args->descomprimir) operacion = 'd';
        else if (args->encriptar) operacion = 'e';
        else if (args->desencriptar) operacion = 'u';
        
        int resultado = procesar_directorio(args->archivo_entrada, args->archivo_salida,
                                           operacion, args->algoritmo_comp, 
                                           args->algoritmo_enc, args->clave);
        
        liberar_argumentos(args);
        
        if (resultado == 0) {
            printf("Procesamiento de directorio completado exitosamente\n");
            return 0;
        } else {
            printf("Error en el procesamiento del directorio\n");
            return 1;
        }
    } else if (es_dir == -1) {
        fprintf(stderr, "Error: No se pudo verificar el tipo de entrada '%s'\n", args->archivo_entrada);
        liberar_argumentos(args);
        return 1;
    }
    
    // Si llegamos aquí, es un archivo individual
    
    // Leer el archivo de entrada
    char* contenido_original = NULL;
    size_t tamano_original = 0;
    
    printf("Leyendo archivo: %s\n", args->archivo_entrada);
    if (leer_archivo(args->archivo_entrada, &contenido_original, &tamano_original) != 0) {
        fprintf(stderr, "Error: No se pudo leer el archivo de entrada\n");
        liberar_argumentos(args);
        return 1;
    }
    
    printf("Archivo leído exitosamente: %zu bytes\n", tamano_original);
    
    // Procesar según la operación solicitada
    if (args->comprimir) {
        printf("Iniciando compresión con algoritmo: %s\n", args->algoritmo_comp);
        
        // Verificar que el algoritmo sea RLE
        if (strcmp(args->algoritmo_comp, "rle") != 0) {
            fprintf(stderr, "Error: Solo se soporta el algoritmo 'rle' en esta versión\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Comprimir los datos
        char* datos_comprimidos = NULL;
        size_t tamano_comprimido = 0;
        
        if (comprimir_rle(contenido_original, tamano_original, &datos_comprimidos, &tamano_comprimido) != 0) {
            fprintf(stderr, "Error: No se pudo comprimir el archivo\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Escribir el archivo comprimido
        printf("Escribiendo archivo comprimido: %s\n", args->archivo_salida);
        if (escribir_archivo(args->archivo_salida, datos_comprimidos, tamano_comprimido) != 0) {
            fprintf(stderr, "Error: No se pudo escribir el archivo comprimido\n");
            liberar_datos(contenido_original);
            liberar_datos(datos_comprimidos);
            liberar_argumentos(args);
            return 1;
        }
        
        printf("Compresión completada exitosamente\n");
        liberar_datos(datos_comprimidos);
        
    } else if (args->descomprimir) {
        printf("Iniciando descompresión con algoritmo: %s\n", args->algoritmo_comp);
        
        // Verificar que el algoritmo sea RLE
        if (strcmp(args->algoritmo_comp, "rle") != 0) {
            fprintf(stderr, "Error: Solo se soporta el algoritmo 'rle' en esta versión\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Descomprimir los datos
        char* datos_descomprimidos = NULL;
        size_t tamano_descomprimido = 0;
        
        if (descomprimir_rle(contenido_original, tamano_original, &datos_descomprimidos, &tamano_descomprimido) != 0) {
            fprintf(stderr, "Error: No se pudo descomprimir el archivo\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Escribir el archivo descomprimido
        printf("Escribiendo archivo descomprimido: %s\n", args->archivo_salida);
        if (escribir_archivo(args->archivo_salida, datos_descomprimidos, tamano_descomprimido) != 0) {
            fprintf(stderr, "Error: No se pudo escribir el archivo descomprimido\n");
            liberar_datos(contenido_original);
            liberar_datos(datos_descomprimidos);
            liberar_argumentos(args);
            return 1;
        }
        
        printf("Descompresión completada exitosamente\n");
        liberar_datos(datos_descomprimidos);
        
    } else if (args->encriptar) {
        printf("Iniciando encriptación con algoritmo: %s\n", args->algoritmo_enc);
        
        // Verificar que el algoritmo sea Vigenère
        if (strcmp(args->algoritmo_enc, "vigenere") != 0) {
            fprintf(stderr, "Error: Solo se soporta el algoritmo 'vigenere' en esta versión\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Verificar que se proporcionó una clave
        if (!args->clave) {
            fprintf(stderr, "Error: Se requiere una clave (-k) para encriptación\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Encriptar los datos
        char* datos_encriptados = NULL;
        size_t tamano_encriptado = 0;
        
        if (encriptar_vigenere(contenido_original, tamano_original, args->clave, 
                               &datos_encriptados, &tamano_encriptado) != 0) {
            fprintf(stderr, "Error: No se pudo encriptar el archivo\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Escribir el archivo encriptado
        printf("Escribiendo archivo encriptado: %s\n", args->archivo_salida);
        if (escribir_archivo(args->archivo_salida, datos_encriptados, tamano_encriptado) != 0) {
            fprintf(stderr, "Error: No se pudo escribir el archivo encriptado\n");
            liberar_datos(contenido_original);
            liberar_datos_encriptados(datos_encriptados);
            liberar_argumentos(args);
            return 1;
        }
        
        printf("Encriptación completada exitosamente\n");
        liberar_datos_encriptados(datos_encriptados);
        
    } else if (args->desencriptar) {
        printf("Iniciando desencriptación con algoritmo: %s\n", args->algoritmo_enc);
        
        // Verificar que el algoritmo sea Vigenère
        if (strcmp(args->algoritmo_enc, "vigenere") != 0) {
            fprintf(stderr, "Error: Solo se soporta el algoritmo 'vigenere' en esta versión\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Verificar que se proporcionó una clave
        if (!args->clave) {
            fprintf(stderr, "Error: Se requiere una clave (-k) para desencriptación\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Desencriptar los datos
        char* datos_desencriptados = NULL;
        size_t tamano_desencriptado = 0;
        
        if (desencriptar_vigenere(contenido_original, tamano_original, args->clave,
                                  &datos_desencriptados, &tamano_desencriptado) != 0) {
            fprintf(stderr, "Error: No se pudo desencriptar el archivo\n");
            liberar_datos(contenido_original);
            liberar_argumentos(args);
            return 1;
        }
        
        // Escribir el archivo desencriptado
        printf("Escribiendo archivo desencriptado: %s\n", args->archivo_salida);
        if (escribir_archivo(args->archivo_salida, datos_desencriptados, tamano_desencriptado) != 0) {
            fprintf(stderr, "Error: No se pudo escribir el archivo desencriptado\n");
            liberar_datos(contenido_original);
            liberar_datos_encriptados(datos_desencriptados);
            liberar_argumentos(args);
            return 1;
        }
        
        printf("Desencriptación completada exitosamente\n");
        liberar_datos_encriptados(datos_desencriptados);
    }
    
    // Limpiar memoria
    liberar_datos(contenido_original);
    liberar_argumentos(args);
    
    printf("Operación completada exitosamente\n");
    return 0;
}
