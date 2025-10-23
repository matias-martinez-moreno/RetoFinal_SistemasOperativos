#include "../include/file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * Lee un archivo completo usando llamadas al sistema
 */
int leer_archivo(const char* ruta, char** contenido, size_t* tamano) {
    if (!ruta || !contenido || !tamano) {
        fprintf(stderr, "Error: Parámetros inválidos para leer_archivo\n");
        return -1;
    }
    
    // Abrir archivo en modo lectura
    int fd = open(ruta, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: No se pudo abrir el archivo '%s': %s\n", ruta, strerror(errno));
        return -1;
    }
    
    // Obtener tamaño del archivo
    struct stat st;
    if (fstat(fd, &st) == -1) {
        fprintf(stderr, "Error: No se pudo obtener información del archivo '%s': %s\n", ruta, strerror(errno));
        close(fd);
        return -1;
    }
    
    *tamano = st.st_size;
    
    // Si el archivo está vacío, retornar éxito con contenido vacío
    if (*tamano == 0) {
        *contenido = malloc(1);
        if (!*contenido) {
            fprintf(stderr, "Error: No se pudo asignar memoria\n");
            close(fd);
            return -1;
        }
        (*contenido)[0] = '\0';
        close(fd);
        return 0;
    }
    
    // Asignar memoria para el contenido
    *contenido = malloc(*tamano + 1);
    if (!*contenido) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el archivo\n");
        close(fd);
        return -1;
    }
    
    // Leer el archivo completo
    ssize_t bytes_leidos = read(fd, *contenido, *tamano);
    if (bytes_leidos == -1) {
        fprintf(stderr, "Error: No se pudo leer el archivo '%s': %s\n", ruta, strerror(errno));
        free(*contenido);
        close(fd);
        return -1;
    }
    
    if (bytes_leidos != (ssize_t)*tamano) {
        fprintf(stderr, "Error: No se leyeron todos los bytes del archivo\n");
        free(*contenido);
        close(fd);
        return -1;
    }
    
    // Agregar terminador nulo
    (*contenido)[*tamano] = '\0';
    
    close(fd);
    return 0;
}

/**
 * Escribe contenido a un archivo usando llamadas al sistema
 */
int escribir_archivo(const char* ruta, const char* contenido, size_t tamano) {
    if (!ruta || !contenido) {
        fprintf(stderr, "Error: Parámetros inválidos para escribir_archivo\n");
        return -1;
    }
    
    // Abrir archivo en modo escritura (crear si no existe, truncar si existe)
    int fd = open(ruta, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error: No se pudo crear/abrir el archivo '%s': %s\n", ruta, strerror(errno));
        return -1;
    }
    
    // Escribir el contenido
    ssize_t bytes_escritos = write(fd, contenido, tamano);
    if (bytes_escritos == -1) {
        fprintf(stderr, "Error: No se pudo escribir al archivo '%s': %s\n", ruta, strerror(errno));
        close(fd);
        return -1;
    }
    
    if (bytes_escritos != (ssize_t)tamano) {
        fprintf(stderr, "Error: No se escribieron todos los bytes al archivo\n");
        close(fd);
        return -1;
    }
    
    // Sincronizar el archivo con el disco
    if (fsync(fd) == -1) {
        fprintf(stderr, "Advertencia: No se pudo sincronizar el archivo con el disco: %s\n", strerror(errno));
    }
    
    close(fd);
    return 0;
}

/**
 * Verifica si un archivo existe
 */
int archivo_existe(const char* ruta) {
    if (!ruta) {
        return -1;
    }
    
    struct stat st;
    if (stat(ruta, &st) == -1) {
        if (errno == ENOENT) {
            return 0; // El archivo no existe
        }
        return -1; // Error al acceder al archivo
    }
    
    return 1; // El archivo existe
}

/**
 * Obtiene el tamaño de un archivo
 */
ssize_t obtener_tamano_archivo(const char* ruta) {
    if (!ruta) {
        return -1;
    }
    
    struct stat st;
    if (stat(ruta, &st) == -1) {
        fprintf(stderr, "Error: No se pudo obtener información del archivo '%s': %s\n", ruta, strerror(errno));
        return -1;
    }
    
    return st.st_size;
}
