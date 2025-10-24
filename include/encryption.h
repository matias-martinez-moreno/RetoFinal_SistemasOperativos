#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stddef.h>

/**
 * Encripta datos usando el algoritmo Vigenère
 * 
 * El algoritmo Vigenère es un método de cifrado polialfabético que usa una clave
 * para determinar qué alfabeto de sustitución usar para cada carácter del texto.
 * 
 * @param datos Datos originales a encriptar
 * @param tamano_original Tamaño de los datos originales
 * @param clave Clave secreta para la encriptación
 * @param datos_encriptados Puntero donde se almacenarán los datos encriptados
 * @param tamano_encriptado Puntero donde se almacenará el tamaño encriptado
 * @return 0 si es exitoso, -1 si hay error
 */
int encriptar_vigenere(const char* datos, size_t tamano_original, const char* clave,
                       char** datos_encriptados, size_t* tamano_encriptado);

/**
 * Desencripta datos usando el algoritmo Vigenère
 * 
 * Reconstruye los datos originales a partir de los datos encriptados
 * usando la misma clave que se usó para encriptar.
 * 
 * @param datos_encriptados Datos encriptados
 * @param tamano_encriptado Tamaño de los datos encriptados
 * @param clave Clave secreta para la desencriptación
 * @param datos_originales Puntero donde se almacenarán los datos originales
 * @param tamano_original Puntero donde se almacenará el tamaño original
 * @return 0 si es exitoso, -1 si hay error
 */
int desencriptar_vigenere(const char* datos_encriptados, size_t tamano_encriptado, const char* clave,
                          char** datos_originales, size_t* tamano_original);

/**
 * Valida que una clave sea válida para encriptación
 * 
 * Una clave válida debe contener solo letras (a-z, A-Z) y no estar vacía.
 * 
 * @param clave Clave a validar
 * @return 1 si es válida, 0 si no es válida
 */
int validar_clave(const char* clave);

/**
 * Libera la memoria asignada para datos encriptados o desencriptados
 * 
 * @param datos Puntero a los datos a liberar
 */
void liberar_datos_encriptados(char* datos);

#endif
