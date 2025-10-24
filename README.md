# GSEA - Utilidad de Gestión Segura y Eficiente de Archivos

## Equipo de Desarrollo
- **Matías Martínez**
- **Sofía Gallo**
- **Juan Manuel Gallo**

## Descripción
Utilidad de línea de comandos para comprimir/descomprimir y encriptar/desencriptar archivos y directorios. Implementa algoritmos propios de compresión RLE y encriptación Vigenère usando llamadas directas al sistema operativo.

## Cómo Probar

### Compilación
```bash
make
```

### Probar con archivos individuales
```bash
# Crear archivo de prueba
echo "AAAABBBCCDDDD" > test.txt

# Comprimir archivo
./gsea -c --comp-alg rle -i test.txt -o test.txt.rle

# Descomprimir archivo
./gsea -d --comp-alg rle -i test.txt.rle -o test_descomprimido.txt

# Encriptar archivo
./gsea -e --enc-alg vigenere -i test.txt -o test.txt.enc -k "clave"

# Desencriptar archivo
./gsea -u --enc-alg vigenere -i test.txt.enc -o test_desencriptado.txt -k "clave"
```

### Probar con directorios
```bash
# Crear directorio de prueba
mkdir test_dir
echo "contenido1" > test_dir/archivo1.txt
echo "contenido2" > test_dir/archivo2.txt

# Comprimir directorio completo
./gsea -c --comp-alg rle -i test_dir -o test_dir_comprimido

# Encriptar directorio completo
./gsea -e --enc-alg vigenere -i test_dir -o test_dir_encriptado -k "clave"
```

## Funcionalidades Implementadas

### Algoritmos Core
- **Compresión RLE**: Algoritmo Run-Length Encoding implementado desde cero
- **Encriptación Vigenère**: Cifrado polialfabético con clave secreta
- **Llamadas al Sistema**: open(), read(), write(), close(), opendir(), readdir()

### Procesamiento
- **Archivos individuales**: Compresión/descompresión y encriptación/desencriptación
- **Directorios completos**: Procesamiento automático de todos los archivos
- **Parser de argumentos**: Manejo completo de parámetros de línea de comandos
- **Gestión de memoria**: Liberación adecuada de recursos
- **Manejo de errores**: Validación robusta de entrada

## Algoritmos Implementados

### RLE (Run-Length Encoding)
- **Función**: Compresión sin pérdida para datos repetitivos
- **Ejemplo**: `AAAABBBCC` → `A4B3C2` (35.71% de reducción)
- **Ventajas**: Simple, rápido, eficaz con datos repetitivos

### Vigenère
- **Función**: Cifrado polialfabético con clave secreta
- **Ejemplo**: "Hola" con clave "abc" → "Hpnc"
- **Ventajas**: Seguro, reversible, clave reutilizable

## Estructura del Proyecto
```
gsea/
├── src/
│   ├── main.c                    # Función principal
│   ├── args.c                    # Parser de argumentos
│   ├── file_manager.c            # Manejo de archivos con llamadas al sistema
│   ├── compression.c             # Algoritmo RLE
│   ├── encryption.c              # Algoritmo Vigenère
│   └── directory_processor.c     # Procesamiento de directorios
├── include/
│   ├── args.h
│   ├── file_manager.h
│   ├── compression.h
│   ├── encryption.h
│   └── directory_processor.h
├── Makefile
└── README.md
```

## Estado del Proyecto

### Completado (40% del proyecto total)
- **Fase 1**: Estructura base (100% completada)
- **Fase 2**: Algoritmos core (100% completada)
- **Procesamiento de directorios**: Implementado con opendir/readdir
- **Llamadas al sistema**: open, read, write, close, opendir, readdir
- **Algoritmos propios**: RLE y Vigenère implementados desde cero

### Faltante (60% del proyecto total)
- **Concurrencia**: Procesamiento paralelo con pthreads (0% implementado)
- **Operaciones combinadas**: -ce, -de, -ec, -du (0% implementado)
- **Optimización**: Rendimiento y memoria (0% implementado)
- **Testing**: Casos de prueba automatizados (0% implementado)
- **Documentación**: Documento técnico completo (0% implementado)

## Requisitos
- Sistema operativo Linux/Unix
- Compilador GCC
- Make
- pthreads (para futuras versiones)

## Caso de Uso
Ideal para startups de biotecnología que manejan datos genéticos repetitivos y confidenciales, permitiendo compresión eficiente y encriptación segura de archivos y directorios masivos.
