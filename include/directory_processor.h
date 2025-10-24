#ifndef DIRECTORY_PROCESSOR_H
#define DIRECTORY_PROCESSOR_H

#include <stddef.h>

/**
 * Procesa un directorio completo aplicando la operación especificada
 * 
 * Esta función procesa todos los archivos regulares dentro de un directorio,
 * aplicando la operación de compresión, descompresión, encriptación o desencriptación
 * según los parámetros proporcionados.
 * 
 * @param ruta_directorio Ruta del directorio a procesar
 * @param ruta_salida Ruta del directorio de salida
 * @param operacion Operación a realizar ('c', 'd', 'e', 'u')
 * @param algoritmo_comp Algoritmo de compresión (solo 'rle' disponible)
 * @param algoritmo_enc Algoritmo de encriptación (solo 'vigenere' disponible)
 * @param clave Clave para encriptación (opcional)
 * @return 0 si es exitoso, -1 si hay error
 */
int procesar_directorio(const char* ruta_directorio, const char* ruta_salida,
                        char operacion, const char* algoritmo_comp, 
                        const char* algoritmo_enc, const char* clave);

/**
 * Lista todos los archivos regulares en un directorio
 * 
 * @param ruta_directorio Ruta del directorio a listar
 * @param archivos Array donde se almacenarán las rutas de los archivos
 * @param num_archivos Puntero donde se almacenará el número de archivos
 * @return 0 si es exitoso, -1 si hay error
 */
int listar_archivos_directorio(const char* ruta_directorio, char*** archivos, size_t* num_archivos);

/**
 * Crea un directorio de salida si no existe
 * 
 * @param ruta_directorio Ruta del directorio a crear
 * @return 0 si es exitoso, -1 si hay error
 */
int crear_directorio_salida(const char* ruta_directorio);

/**
 * Verifica si una ruta es un directorio
 * 
 * @param ruta Ruta a verificar
 * @return 1 si es directorio, 0 si no, -1 si hay error
 */
int es_directorio(const char* ruta);

/**
 * Libera la memoria asignada para la lista de archivos
 * 
 * @param archivos Array de archivos a liberar
 * @param num_archivos Número de archivos en el array
 */
void liberar_lista_archivos(char** archivos, size_t num_archivos);

/**
 * Procesa un archivo individual aplicando la operación especificada
 * 
 * @param archivo_entrada Ruta del archivo de entrada
 * @param archivo_salida Ruta del archivo de salida
 * @param operacion Operación a realizar ('c', 'd', 'e', 'u')
 * @param algoritmo_comp Algoritmo de compresión
 * @param algoritmo_enc Algoritmo de encriptación
 * @param clave Clave para encriptación
 * @return 0 si es exitoso, -1 si hay error
 */
int procesar_archivo_individual(const char* archivo_entrada, const char* archivo_salida,
                               char operacion, const char* algoritmo_comp,
                               const char* algoritmo_enc, const char* clave);

#endif
