# GSEA - Utilidad de Gestión Segura y Eficiente de Archivos

## Descripción
Utilidad de línea de comandos para comprimir/descomprimir y encriptar/desencriptar archivos. Implementa algoritmos propios de compresión RLE y encriptación Vigenère usando llamadas directas al sistema operativo.

## Características Implementadas
- **Compresión RLE**: Algoritmo Run-Length Encoding implementado desde cero
- **Encriptación Vigenère**: Cifrado polialfabético con clave secreta
- **Llamadas al Sistema**: Uso directo de open(), read(), write(), close()
- **Parser de Argumentos**: Manejo completo de parámetros de línea de comandos
- **Gestión de Memoria**: Liberación adecuada de recursos
- **Manejo de Errores**: Validación robusta de entrada

## Compilación
```bash
make
```

## Uso

### Compresión/Descompresión
```bash
# Comprimir un archivo
./gsea -c --comp-alg rle -i archivo.txt -o archivo.txt.rle

# Descomprimir un archivo
./gsea -d --comp-alg rle -i archivo.txt.rle -o archivo_descomprimido.txt
```

### Encriptación/Desencriptación
```bash
# Encriptar un archivo
./gsea -e --enc-alg vigenere -i archivo.txt -o archivo.txt.enc -k "clave"

# Desencriptar un archivo
./gsea -u --enc-alg vigenere -i archivo.txt.enc -o archivo_desencriptado.txt -k "clave"
```

### Ayuda
```bash
./gsea --help
```

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
│   ├── main.c              # Función principal
│   ├── args.c              # Parser de argumentos
│   ├── file_manager.c      # Manejo de archivos con llamadas al sistema
│   ├── compression.c       # Algoritmo RLE
│   └── encryption.c        # Algoritmo Vigenère
├── include/
│   ├── args.h
│   ├── file_manager.h
│   ├── compression.h
│   └── encryption.h
├── Makefile
└── README.md
```

## Estado del Proyecto
- **Fase 1**: Estructura base (100% completada)
- **Fase 2**: Algoritmos core (100% completada)
- **Fase 3**: Concurrencia (0% - próxima versión)
- **Fase 4**: Integración avanzada (0% - próxima versión)

## Requisitos
- Sistema operativo Linux/Unix
- Compilador GCC
- Make
- pthreads (para futuras versiones)

## Caso de Uso
Ideal para startups de biotecnología que manejan datos genéticos repetitivos y confidenciales, permitiendo compresión eficiente y encriptación segura de archivos masivos.
