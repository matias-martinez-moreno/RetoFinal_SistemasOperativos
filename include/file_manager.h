#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <sys/types.h>

/**
 * Lee un archivo completo usando llamadas al sistema
 * @param ruta Ruta del archivo a leer
 * @param contenido Puntero donde se almacenará el contenido
 * @param tamano Puntero donde se almacenará el tamaño del archivo
 * @return 0 si es exitoso, -1 si hay error
 */
int leer_archivo(const char* ruta, char** contenido, size_t* tamano);

/**
 * Escribe contenido a un archivo usando llamadas al sistema
 * @param ruta Ruta del archivo a escribir
 * @param contenido Contenido a escribir
 * @param tamano Tamaño del contenido
 * @return 0 si es exitoso, -1 si hay error
 */
int escribir_archivo(const char* ruta, const char* contenido, size_t tamano);

/**
 * Verifica si un archivo existe
 * @param ruta Ruta del archivo a verificar
 * @return 1 si existe, 0 si no existe, -1 si hay error
 */
int archivo_existe(const char* ruta);

/**
 * Obtiene el tamaño de un archivo
 * @param ruta Ruta del archivo
 * @return Tamaño del archivo en bytes, -1 si hay error
 */
ssize_t obtener_tamano_archivo(const char* ruta);

#endif
