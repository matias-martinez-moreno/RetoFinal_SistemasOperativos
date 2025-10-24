#include "../include/compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Comprime datos usando el algoritmo RLE (Run-Length Encoding)
 * 
 * El algoritmo RLE (Run-Length Encoding) es un método de compresión sin pérdida
 * que funciona contando secuencias consecutivas del mismo carácter y las codifica
 * como: [carácter][contador]. Es especialmente eficaz con datos que contienen
 * muchas repeticiones consecutivas.
 * 
 * Ejemplo de funcionamiento:
 * - Entrada: "AAAABBBCC"
 * - Proceso: A(4) + B(3) + C(2)
 * - Salida: "A4B3C2"
 * 
 * Ventajas del algoritmo RLE:
 * - Simple de implementar
 * - Eficiente para datos repetitivos
 * - Compresión sin pérdida
 * - Tiempo de procesamiento O(n)
 */
int comprimir_rle(const char* datos, size_t tamano_original, 
                  char** datos_comprimidos, size_t* tamano_comprimido) {
    if (!datos || !datos_comprimidos || !tamano_comprimido || tamano_original == 0) {
        fprintf(stderr, "Error: Parámetros inválidos para comprimir_rle\n");
        return -1;
    }
    
    // Buffer temporal para almacenar la compresión
    char* buffer = malloc(tamano_original * 2 + 1); // Peor caso: cada carácter se duplica
    if (!buffer) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la compresión\n");
        return -1;
    }
    
    size_t pos_buffer = 0;
    size_t i = 0;
    
    while (i < tamano_original) {
        char caracter_actual = datos[i];
        int contador = 1;
        
        // Contar caracteres consecutivos iguales
        while (i + contador < tamano_original && datos[i + contador] == caracter_actual) {
            contador++;
        }
        
        // Escribir el carácter y su contador al buffer
        if (contador == 1) {
            // Si solo hay un carácter, escribirlo directamente
            buffer[pos_buffer++] = caracter_actual;
        } else {
            // Si hay múltiples caracteres, escribir carácter + contador
            buffer[pos_buffer++] = caracter_actual;
            buffer[pos_buffer++] = (char)contador;
        }
        
        i += contador;
    }
    
    // Asignar memoria exacta para el resultado
    *datos_comprimidos = malloc(pos_buffer);
    if (!*datos_comprimidos) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el resultado\n");
        free(buffer);
        return -1;
    }
    
    // Copiar el resultado
    memcpy(*datos_comprimidos, buffer, pos_buffer);
    *tamano_comprimido = pos_buffer;
    
    free(buffer);
    
    printf("Compresión RLE completada: %zu bytes -> %zu bytes (%.2f%% de reducción)\n", 
           tamano_original, *tamano_comprimido, 
           (1.0 - (double)*tamano_comprimido / tamano_original) * 100);
    
    return 0;
}

/**
 * Descomprime datos usando el algoritmo RLE
 * 
 * Reconstruye los datos originales a partir de la codificación RLE
 * Ejemplo: "A4B3C2" se convierte en "AAAABBBCC"
 */
int descomprimir_rle(const char* datos_comprimidos, size_t tamano_comprimido,
                     char** datos_originales, size_t* tamano_original) {
    if (!datos_comprimidos || !datos_originales || !tamano_original || tamano_comprimido == 0) {
        fprintf(stderr, "Error: Parámetros inválidos para descomprimir_rle\n");
        return -1;
    }
    
    // Buffer temporal para almacenar la descompresión
    char* buffer = malloc(tamano_comprimido * 10); // Estimación generosa
    if (!buffer) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la descompresión\n");
        return -1;
    }
    
    size_t pos_buffer = 0;
    size_t i = 0;
    
    while (i < tamano_comprimido) {
        char caracter = datos_comprimidos[i++];
        
        if (i < tamano_comprimido) {
            char siguiente = datos_comprimidos[i];
            
            // Si el siguiente carácter es un número (contador)
            if (siguiente >= '1' && siguiente <= '9') {
                int contador = siguiente - '0';
                i++; // Saltar el contador
                
                // Repetir el carácter el número de veces indicado
                for (int j = 0; j < contador; j++) {
                    buffer[pos_buffer++] = caracter;
                }
            } else {
                // Si no hay contador, solo escribir el carácter una vez
                buffer[pos_buffer++] = caracter;
            }
        } else {
            // Último carácter sin contador
            buffer[pos_buffer++] = caracter;
        }
    }
    
    // Asignar memoria exacta para el resultado
    *datos_originales = malloc(pos_buffer + 1);
    if (!*datos_originales) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el resultado\n");
        free(buffer);
        return -1;
    }
    
    // Copiar el resultado y agregar terminador nulo
    memcpy(*datos_originales, buffer, pos_buffer);
    (*datos_originales)[pos_buffer] = '\0';
    *tamano_original = pos_buffer;
    
    free(buffer);
    
    printf("Descompresión RLE completada: %zu bytes -> %zu bytes\n", 
           tamano_comprimido, *tamano_original);
    
    return 0;
}

/**
 * Libera la memoria asignada para datos comprimidos o descomprimidos
 */
void liberar_datos(char* datos) {
    if (datos) {
        free(datos);
    }
}
