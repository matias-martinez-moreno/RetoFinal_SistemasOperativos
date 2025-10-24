#include "../include/encryption.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Encripta datos usando el algoritmo Vigenère
 * 
 * El algoritmo Vigenère funciona aplicando un desplazamiento a cada carácter
 * del texto original. El desplazamiento se determina por el carácter correspondiente
 * en la clave, que se repite cíclicamente.
 * 
 * Fórmula: C = (P + K) mod 26
 * Donde:
 * - C = carácter encriptado
 * - P = carácter original
 * - K = carácter de la clave
 */
int encriptar_vigenere(const char* datos, size_t tamano_original, const char* clave,
                       char** datos_encriptados, size_t* tamano_encriptado) {
    // Verificar que los parámetros sean válidos
    if (!datos || !clave || !datos_encriptados || !tamano_encriptado) {
        fprintf(stderr, "Error: Parámetros inválidos para encriptar_vigenere\n");
        return -1;
    }
    
    // Validar la clave
    if (!validar_clave(clave)) {
        fprintf(stderr, "Error: La clave no es válida. Debe contener solo letras.\n");
        return -1;
    }
    
    // Asignar memoria para los datos encriptados
    *datos_encriptados = malloc(tamano_original + 1);
    if (!*datos_encriptados) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la encriptación\n");
        return -1;
    }
    
    size_t longitud_clave = strlen(clave);
    size_t posicion_clave = 0; // Posición actual en la clave
    
    // Procesar cada carácter del texto original
    for (size_t i = 0; i < tamano_original; i++) {
        char caracter_original = datos[i];
        
        // Solo encriptar letras (a-z, A-Z)
        if (isalpha(caracter_original)) {
            // Determinar si es mayúscula o minúscula
            int es_mayuscula = isupper(caracter_original);
            char base = es_mayuscula ? 'A' : 'a';
            
            // Obtener el carácter de la clave (cíclicamente)
            char caracter_clave = clave[posicion_clave % longitud_clave];
            caracter_clave = tolower(caracter_clave); // Convertir a minúscula para consistencia
            
            // Calcular el desplazamiento
            int desplazamiento = caracter_clave - 'a';
            
            // Aplicar el cifrado Vigenère
            char caracter_encriptado = ((caracter_original - base + desplazamiento) % 26) + base;
            
            (*datos_encriptados)[i] = caracter_encriptado;
            
            // Avanzar en la clave
            posicion_clave++;
        } else {
            // Mantener caracteres que no son letras sin cambios
            (*datos_encriptados)[i] = caracter_original;
        }
    }
    
    // Agregar terminador nulo
    (*datos_encriptados)[tamano_original] = '\0';
    *tamano_encriptado = tamano_original;
    
    printf("Encriptación Vigenère completada: %zu bytes procesados\n", tamano_original);
    
    return 0;
}

/**
 * Desencripta datos usando el algoritmo Vigenère
 * 
 * Para desencriptar, se aplica la operación inversa del cifrado:
 * P = (C - K + 26) mod 26
 * Donde:
 * - P = carácter original
 * - C = carácter encriptado
 * - K = carácter de la clave
 */
int desencriptar_vigenere(const char* datos_encriptados, size_t tamano_encriptado, const char* clave,
                          char** datos_originales, size_t* tamano_original) {
    // Verificar que los parámetros sean válidos
    if (!datos_encriptados || !clave || !datos_originales || !tamano_original) {
        fprintf(stderr, "Error: Parámetros inválidos para desencriptar_vigenere\n");
        return -1;
    }
    
    // Validar la clave
    if (!validar_clave(clave)) {
        fprintf(stderr, "Error: La clave no es válida. Debe contener solo letras.\n");
        return -1;
    }
    
    // Asignar memoria para los datos originales
    *datos_originales = malloc(tamano_encriptado + 1);
    if (!*datos_originales) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la desencriptación\n");
        return -1;
    }
    
    size_t longitud_clave = strlen(clave);
    size_t posicion_clave = 0; // Posición actual en la clave
    
    // Procesar cada carácter del texto encriptado
    for (size_t i = 0; i < tamano_encriptado; i++) {
        char caracter_encriptado = datos_encriptados[i];
        
        // Solo desencriptar letras (a-z, A-Z)
        if (isalpha(caracter_encriptado)) {
            // Determinar si es mayúscula o minúscula
            int es_mayuscula = isupper(caracter_encriptado);
            char base = es_mayuscula ? 'A' : 'a';
            
            // Obtener el carácter de la clave (cíclicamente)
            char caracter_clave = clave[posicion_clave % longitud_clave];
            caracter_clave = tolower(caracter_clave); // Convertir a minúscula para consistencia
            
            // Calcular el desplazamiento
            int desplazamiento = caracter_clave - 'a';
            
            // Aplicar el descifrado Vigenère (operación inversa)
            char caracter_original = ((caracter_encriptado - base - desplazamiento + 26) % 26) + base;
            
            (*datos_originales)[i] = caracter_original;
            
            // Avanzar en la clave
            posicion_clave++;
        } else {
            // Mantener caracteres que no son letras sin cambios
            (*datos_originales)[i] = caracter_encriptado;
        }
    }
    
    // Agregar terminador nulo
    (*datos_originales)[tamano_encriptado] = '\0';
    *tamano_original = tamano_encriptado;
    
    printf("Desencriptación Vigenère completada: %zu bytes procesados\n", *tamano_original);
    
    return 0;
}

/**
 * Valida que una clave sea válida para encriptación
 * 
 * Una clave válida debe:
 * - No estar vacía
 * - Contener solo letras (a-z, A-Z)
 * - Tener al menos 1 carácter
 */
int validar_clave(const char* clave) {
    if (!clave || strlen(clave) == 0) {
        return 0; // Clave vacía o nula
    }
    
    // Verificar que todos los caracteres sean letras
    for (size_t i = 0; i < strlen(clave); i++) {
        if (!isalpha(clave[i])) {
            return 0; // Contiene caracteres que no son letras
        }
    }
    
    return 1; // Clave válida
}

/**
 * Libera la memoria asignada para datos encriptados o desencriptados
 */
void liberar_datos_encriptados(char* datos) {
    if (datos) {
        free(datos);
    }
}
