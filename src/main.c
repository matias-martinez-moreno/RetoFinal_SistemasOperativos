#include "../include/args.h"
#include "../include/file_manager.h"
#include "../include/compression.h"
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
    printf("Versión: 1.0 (Avance - Solo Compresión RLE)\n\n");
    
    // Parsear argumentos de línea de comandos
    Argumentos* args = parsear_argumentos(argc, argv);
    if (!args) {
        return 1;
    }
    
    // Verificar que el archivo de entrada existe
    int existe = archivo_existe(args->archivo_entrada);
    if (existe == 0) {
        fprintf(stderr, "Error: El archivo de entrada '%s' no existe\n", args->archivo_entrada);
        liberar_argumentos(args);
        return 1;
    } else if (existe == -1) {
        fprintf(stderr, "Error: No se pudo acceder al archivo de entrada '%s'\n", args->archivo_entrada);
        liberar_argumentos(args);
        return 1;
    }
    
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
        
    } else if (args->encriptar || args->desencriptar) {
        fprintf(stderr, "Error: Las operaciones de encriptación no están implementadas en esta versión\n");
        fprintf(stderr, "Esta es una versión de avance que solo soporta compresión/descompresión\n");
        liberar_datos(contenido_original);
        liberar_argumentos(args);
        return 1;
    }
    
    // Limpiar memoria
    liberar_datos(contenido_original);
    liberar_argumentos(args);
    
    printf("Operación completada exitosamente\n");
    return 0;
}
