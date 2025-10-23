#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

/**
 * Estructura para almacenar los argumentos parseados de la línea de comandos
 */
typedef struct {
    bool comprimir;        // -c: comprimir archivo
    bool descomprimir;     // -d: descomprimir archivo
    bool encriptar;        // -e: encriptar archivo (no implementado aún)
    bool desencriptar;     // -u: desencriptar archivo (no implementado aún)
    
    char* algoritmo_comp;  // --comp-alg: algoritmo de compresión
    char* algoritmo_enc;   // --enc-alg: algoritmo de encriptación (no implementado)
    char* archivo_entrada; // -i: archivo de entrada
    char* archivo_salida;  // -o: archivo de salida
    char* clave;           // -k: clave para encriptación (no implementado)
} Argumentos;

/**
 * Parsea los argumentos de la línea de comandos
 * @param argc Número de argumentos
 * @param argv Array de argumentos
 * @return Estructura con los argumentos parseados, NULL si hay error
 */
Argumentos* parsear_argumentos(int argc, char* argv[]);

/**
 * Libera la memoria asignada a los argumentos
 * @param args Estructura de argumentos a liberar
 */
void liberar_argumentos(Argumentos* args);

/**
 * Imprime el uso del programa
 */
void mostrar_ayuda(void);

#endif
