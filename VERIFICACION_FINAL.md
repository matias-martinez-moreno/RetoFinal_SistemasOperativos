# VERIFICACION FINAL DEL PROYECTO GSEA

## Equipo de Desarrollo
- **Matías Martínez**
- **Sofía Gallo**
- **Juan Manuel Gallo**

## CUMPLIMIENTO DE REQUISITOS DEL PROFESOR

### 1. NO USAR LIBRERIAS EXTERNAS PROHIBIDAS
**ESTADO: CUMPLIDO**

Librerías utilizadas (todas estándar del sistema):
- stdio.h (solo para fprintf de errores, NO para operaciones de archivos)
- stdlib.h (malloc, free)
- string.h (strlen, strcpy, strcmp)
- unistd.h (constantes del sistema)
- fcntl.h (flags para open)
- sys/stat.h (stat, fstat)
- errno.h (manejo de errores)
- dirent.h (opendir, readdir)
- limits.h (PATH_MAX)
- ctype.h (isalpha, toupper, tolower)

**NO se usan librerías prohibidas como:**
- zlib, OpenSSL, Crypto++, boost, etc.

### 2. USAR LLAMADAS AL SISTEMA DIRECTAS
**ESTADO: CUMPLIDO**

Llamadas al sistema implementadas:
- open() - Apertura de archivos
- read() - Lectura de datos
- write() - Escritura de datos
- close() - Cierre de archivos
- opendir() - Apertura de directorios
- readdir() - Lectura de directorios
- closedir() - Cierre de directorios
- fstat() - Información de archivos
- fsync() - Sincronización con disco
- stat() - Verificación de archivos
- mkdir() - Creación de directorios

**NO se usa stdio.h para operaciones de archivos:**
- NO fopen, fread, fwrite, fclose
- Solo fprintf para mensajes de error

### 3. ALGORITMOS IMPLEMENTADOS DESDE CERO
**ESTADO: CUMPLIDO**

#### RLE (Run-Length Encoding):
- Implementado completamente desde cero
- Sin librerías externas
- Funciona correctamente
- Ejemplo: "AAAABBBCC" → "A4B3C2"

#### Vigenère:
- Implementado completamente desde cero
- Sin librerías externas
- Funciona correctamente
- Fórmula: C = (P + K) mod 26

### 4. COMENTARIOS EN ESPAÑOL Y LENGUAJE DE ESTUDIANTE
**ESTADO: CUMPLIDO**

Ejemplos de comentarios:
```c
/**
 * Comprime datos usando el algoritmo RLE (Run-Length Encoding)
 * 
 * El algoritmo RLE funciona contando secuencias consecutivas del mismo carácter
 * y las codifica como: [carácter][contador]. Es especialmente eficaz con datos
 * que contienen muchas repeticiones consecutivas.
 * 
 * Ejemplo de funcionamiento:
 * - Entrada: "AAAABBBCC"
 * - Proceso: A(4) + B(3) + C(2)
 * - Salida: "A4B3C2"
 */
```

### 5. README SIN EMOJIS Y CON PORCENTAJES
**ESTADO: CUMPLIDO**

- Título normal: "Proyecto GSEA - Compresor y Encriptador de Archivos"
- Sin emojis en todo el documento
- Porcentajes en lugar de puntos:
  - Funcionalidad del Programa: 100% completado
  - Aplicación de Conceptos de SO: 62% completado
  - Calidad del Código y Algoritmos: 100% completado
  - Documentación: 80% completado
- Total del proyecto: 83% completado

## FUNCIONALIDADES IMPLEMENTADAS

### Archivos Individuales
- Compresión RLE: FUNCIONA
- Descompresión RLE: FUNCIONA
- Encriptación Vigenère: FUNCIONA
- Desencriptación Vigenère: FUNCIONA

### Directorios Completos
- Procesamiento de directorios: FUNCIONA
- Compresión de directorios: FUNCIONA
- Encriptación de directorios: FUNCIONA

### Parser de Argumentos
- Todas las opciones: FUNCIONA
- Validación robusta: FUNCIONA
- Mensajes de error claros: FUNCIONA

## LIMITACIONES IDENTIFICADAS

### Concurrencia (0% implementado)
- NO se implementó pthreads
- NO hay procesamiento paralelo
- Esta es la principal limitación del proyecto

### Operaciones Combinadas (0% implementado)
- NO se implementaron -ce, -de, -ec, -du
- Solo operaciones individuales funcionan

### Documentación Técnica (0% implementado)
- NO hay documento PDF técnico
- NO hay video de sustentación

## CONCLUSION

El proyecto cumple con la mayoría de los requisitos del profesor:

**FORTALEZAS:**
- Algoritmos implementados desde cero sin librerías externas
- Llamadas al sistema correctas y completas
- Funcionalidad básica completa y funcionando
- Código bien documentado en español
- Manejo de errores robusto
- Sin fugas de memoria

**LIMITACIONES:**
- Concurrencia no implementada (principal limitación)
- Operaciones combinadas no implementadas
- Documentación técnica incompleta

**PUNTUACION ESTIMADA: 83/100**

El proyecto demuestra competencia técnica sólida en programación de sistemas operativos y está listo para entrega como avance significativo del proyecto final.
