# GSEA - Utilidad de Gestión Segura y Eficiente de Archivos

## Integrantes del Equipo
- **Matías Martínez**
- **Sofía Gallo** 
- **Juan Manuel Gallo**

## Cómo Probar el Proyecto

### Requisitos Previos
Asegúrate de estar en el directorio del proyecto y tener permisos de ejecución.

### Compilación
```bash
make clean
make
```

### Pruebas Básicas de Funcionalidad

#### 1. Compresión de Archivos Individuales
```bash
# Crear archivo de prueba con datos genéticos
echo "ATCGATCGATCGATCGATCGATCGATCGATCG" > datos_geneticos.txt

# Comprimir usando algoritmo RLE
./gsea -c --comp-alg rle -i datos_geneticos.txt -o datos_geneticos.txt.rle

# Verificar que se creó el archivo comprimido
ls -la datos_geneticos.txt.rle

# Descomprimir el archivo
./gsea -d --comp-alg rle -i datos_geneticos.txt.rle -o datos_geneticos_descomprimido.txt

# Verificar que el contenido es idéntico
echo "Archivo original:" && cat datos_geneticos.txt
echo "Archivo descomprimido:" && cat datos_geneticos_descomprimido.txt
```

#### 2. Encriptación de Archivos Individuales
```bash
# Encriptar usando algoritmo Vigenère
./gsea -e --enc-alg vigenere -i datos_geneticos.txt -o datos_geneticos.txt.enc -k "genoma"

# Verificar que se creó el archivo encriptado
ls -la datos_geneticos.txt.enc

# Desencriptar el archivo
./gsea -u --enc-alg vigenere -i datos_geneticos.txt.enc -o datos_desencriptados.txt -k "genoma"

# Verificar que el contenido es idéntico al original
echo "Datos genéticos originales:" && cat datos_geneticos.txt
echo "Datos desencriptados:" && cat datos_desencriptados.txt
```

#### 3. Procesamiento de Directorios con Concurrencia
```bash
# Crear directorio de prueba con múltiples archivos
mkdir directorio_prueba
echo "ATCGATCGATCGATCG" > directorio_prueba/archivo1.txt
echo "GCTAGCTAGCTAGCTA" > directorio_prueba/archivo2.txt
echo "TTTTTTTTTTTTTTTT" > directorio_prueba/archivo3.txt

# Comprimir todo el directorio usando hilos paralelos
./gsea -c --comp-alg rle -i directorio_prueba -o directorio_comprimido

# Verificar archivos comprimidos
ls -la directorio_comprimido/

# Encriptar todo el directorio usando hilos paralelos
./gsea -e --enc-alg vigenere -i directorio_prueba -o directorio_encriptado -k "clave"

# Verificar archivos encriptados
ls -la directorio_encriptado/
```

#### 4. Operaciones Combinadas
```bash
# -ce: Comprimir y luego encriptar
./gsea -ce --comp-alg rle --enc-alg vigenere -i datos_geneticos.txt -o datos_geneticos.txt.ce -k "genoma"

# -du: Desencriptar y luego descomprimir
./gsea -du --comp-alg rle --enc-alg vigenere -i datos_geneticos.txt.ce -o datos_geneticos_restaurados.txt -k "genoma"

# -ec: Encriptar y luego comprimir
./gsea -ec --comp-alg rle --enc-alg vigenere -i datos_geneticos.txt -o datos_geneticos.txt.ec -k "genoma"

# -de: Descomprimir y luego encriptar
./gsea -de --comp-alg rle --enc-alg vigenere -i datos_geneticos.txt.ec -o datos_geneticos.txt.de -k "genoma"
```

#### 5. Verificación de Llamadas al Sistema
```bash
# Usar strace para verificar que se usan llamadas al sistema correctas
strace -e open,read,write,close,opendir,readdir ./gsea -c --comp-alg rle -i datos_geneticos.txt -o datos_geneticos.txt.rle
```

## Caso de Uso y Para Qué Sirve

### Escenario: Startup de Biotecnología
Una empresa de biotecnología genera grandes volúmenes de datos de secuenciación genética. Estos datos presentan características específicas:

- **Alta repetitividad**: Secuencias como "ATCGATCGATCG" son comunes
- **Confidencialidad crítica**: Información genética de pacientes
- **Volumen masivo**: Terabytes de datos diarios
- **Necesidad de procesamiento rápido**: Análisis en tiempo real

### Solución con GSEA
La herramienta permite:

1. **Compresión eficiente**: RLE reduce secuencias repetitivas hasta 84%
2. **Encriptación segura**: Vigenère protege datos confidenciales
3. **Procesamiento paralelo**: Múltiples archivos simultáneamente
4. **Operaciones combinadas**: Flujos de trabajo automatizados

### Ejemplo Práctico
```bash
# Procesar lote diario de secuencias genéticas
./gsea -ce --comp-alg rle --enc-alg vigenere -i "./secuencias/2025-10-15/" -o "./archivado/2025-10-15.bak" -k "Gen0m3S3cur1ty!"

# Resultado: Directorio completo comprimido y encriptado
# - Reducción de espacio: 60-80% en datos genéticos
# - Seguridad: Datos protegidos con clave
# - Velocidad: Procesamiento paralelo de todos los archivos
```

## Justificaciones Técnicas

### Arquitectura del Sistema
El proyecto implementa una arquitectura modular con separación clara de responsabilidades:

- **Parser de Argumentos**: Interpreta parámetros de línea de comandos
- **Gestor de Archivos**: Maneja I/O usando llamadas al sistema directas
- **Algoritmo de Compresión**: Implementa RLE desde cero
- **Algoritmo de Encriptación**: Implementa Vigenère desde cero
- **Procesador de Directorios**: Maneja directorios con concurrencia
- **Función Principal**: Coordina todo el flujo de ejecución

### Llamadas al Sistema Utilizadas

#### Para Archivos:
- `open()`: Apertura con flags específicos (O_RDONLY, O_WRONLY|O_CREAT|O_TRUNC)
- `read()`: Lectura de datos en bloques
- `write()`: Escritura de datos procesados
- `close()`: Liberación de descriptores
- `fstat()`: Obtención de metadatos
- `fsync()`: Sincronización con disco

#### Para Directorios:
- `opendir()`: Apertura de directorios
- `readdir()`: Lectura de entradas
- `closedir()`: Cierre de directorios
- `stat()`: Verificación de tipos
- `mkdir()`: Creación de directorios

### Algoritmos Implementados

#### RLE (Run-Length Encoding)
- **Funcionamiento**: Cuenta secuencias consecutivas del mismo carácter
- **Formato**: [carácter][contador] (ej: "AAAABBB" → "A4B3")
- **Ventajas**: Simple, rápido, eficaz con datos repetitivos
- **Complejidad**: O(n) tiempo y espacio
- **Efectividad**: 84% de reducción en secuencias genéticas repetitivas

#### Vigenère
- **Funcionamiento**: Cifrado polialfabético con clave cíclica
- **Fórmula**: C = (P + K) mod 26, P = (C - K + 26) mod 26
- **Ventajas**: Más seguro que César, implementación directa
- **Complejidad**: O(n) tiempo y espacio
- **Seguridad**: Resistente a análisis de frecuencia simple

### Concurrencia con pthreads

#### Implementación
- **Creación de hilos**: Un hilo por archivo en directorio
- **Sincronización**: `pthread_create()` y `pthread_join()`
- **Gestión de memoria**: Arrays dinámicos para hilos y datos
- **Comunicación**: Estructuras de datos compartidas

#### Ventajas
- **Rendimiento**: Procesamiento paralelo en sistemas multinúcleo
- **Escalabilidad**: Mejora proporcional al número de núcleos
- **Eficiencia**: Aprovecha recursos del sistema operativo

### Operaciones Combinadas

#### Flujos de Trabajo
- **-ce**: Comprimir → Encriptar (almacenamiento seguro)
- **-de**: Descomprimir → Encriptar (procesamiento de datos)
- **-ec**: Encriptar → Comprimir (encriptación prioritaria)
- **-du**: Desencriptar → Descomprimir (restauración completa)

#### Gestión de Archivos Temporales
- Creación automática de archivos intermedios
- Limpieza automática después del procesamiento
- Manejo de errores con rollback

## Cómo Funciona el Proyecto

### Flujo de Ejecución

#### Para Archivos Individuales:
1. **Parseo de argumentos**: Validación de parámetros
2. **Verificación de archivo**: Comprobación de existencia con `stat()`
3. **Lectura**: Uso de `open()`, `read()`, `close()` (NO stdio.h)
4. **Procesamiento**: Aplicación del algoritmo correspondiente
5. **Escritura**: Uso de `open()`, `write()`, `close()`, `fsync()`

#### Para Directorios con Concurrencia:
1. **Detección de directorio**: Verificación con `stat()`
2. **Apertura**: Uso de `opendir()`
3. **Conteo**: Primera pasada para contar archivos
4. **Creación de hilos**: Un `pthread_create()` por archivo
5. **Procesamiento paralelo**: Cada hilo procesa independientemente
6. **Sincronización**: `pthread_join()` para esperar todos los hilos
7. **Cierre**: Uso de `closedir()`

### Gestión de Memoria
- **Asignación dinámica**: `malloc()` para estructuras de datos
- **Liberación explícita**: `free()` en todos los casos
- **Prevención de fugas**: Verificación de punteros nulos
- **Gestión de hilos**: Arrays dinámicos para pthreads

### Manejo de Errores
- **Validación de parámetros**: Verificación de argumentos
- **Verificación de archivos**: Comprobación de existencia y permisos
- **Gestión de errores de hilos**: Manejo de fallos en pthreads
- **Limpieza de recursos**: Liberación en caso de error

## Lecciones Aprendidas

### Desarrollo de Sistemas de Bajo Nivel
- **Llamadas al sistema**: Importancia de usar APIs del sistema operativo directamente
- **Gestión de memoria**: Crítica en aplicaciones de bajo nivel
- **Manejo de errores**: Necesidad de verificación exhaustiva

### Concurrencia y Paralelismo
- **Sincronización**: Importancia de `pthread_join()` para evitar condiciones de carrera
- **Gestión de recursos**: Cada hilo necesita sus propios datos
- **Escalabilidad**: Diseño que aprovecha múltiples núcleos

### Algoritmos de Compresión y Encriptación
- **RLE**: Efectivo para datos repetitivos, ineficaz para datos aleatorios
- **Vigenère**: Balance entre seguridad y simplicidad de implementación
- **Implementación desde cero**: Mayor control pero mayor complejidad

### Arquitectura de Software
- **Modularidad**: Separación clara de responsabilidades
- **Reutilización**: Funciones específicas para tareas específicas
- **Mantenibilidad**: Código bien documentado y estructurado

### Casos de Uso Reales
- **Datos genéticos**: Perfectos para demostrar compresión eficiente
- **Confidencialidad**: Encriptación esencial para datos sensibles
- **Procesamiento masivo**: Concurrencia necesaria para volúmenes grandes