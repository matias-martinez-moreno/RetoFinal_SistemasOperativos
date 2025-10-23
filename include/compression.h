#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stddef.h>

/**
 * Comprime datos usando el algoritmo RLE (Run-Length Encoding)
 * @param datos Datos originales a comprimir
 * @param tamano_original Tamaño de los datos originales
 * @param datos_comprimidos Puntero donde se almacenarán los datos comprimidos
 * @param tamano_comprimido Puntero donde se almacenará el tamaño comprimido
 * @return 0 si es exitoso, -1 si hay error
 */
int comprimir_rle(const char* datos, size_t tamano_original, 
                  char** datos_comprimidos, size_t* tamano_comprimido);

/**
 * Descomprime datos usando el algoritmo RLE
 * @param datos_comprimidos Datos comprimidos
 * @param tamano_comprimido Tamaño de los datos comprimidos
 * @param datos_originales Puntero donde se almacenarán los datos originales
 * @param tamano_original Puntero donde se almacenará el tamaño original
 * @return 0 si es exitoso, -1 si hay error
 */
int descomprimir_rle(const char* datos_comprimidos, size_t tamano_comprimido,
                     char** datos_originales, size_t* tamano_original);

/**
 * Libera la memoria asignada para datos comprimidos o descomprimidos
 * @param datos Puntero a los datos a liberar
 */
void liberar_datos(char* datos);

#endif
