# GSEA - Utilidad de Gestión Segura y Eficiente de Archivos

## Descripción
Utilidad de línea de comandos para comprimir/descomprimir archivos usando el algoritmo RLE (Run-Length Encoding). Esta es una versión de avance que implementa las funcionalidades básicas del proyecto final.

## Características Implementadas
- ✅ **Compresión/Descompresión RLE**: Algoritmo implementado desde cero
- ✅ **Llamadas al Sistema**: Uso directo de open(), read(), write(), close()
- ✅ **Parser de Argumentos**: Manejo completo de parámetros de línea de comandos
- ✅ **Gestión de Memoria**: Liberación adecuada de recursos
- ✅ **Manejo de Errores**: Validación robusta de entrada

## Compilación
```bash
make
```

## Uso
```bash
# Comprimir un archivo
./gsea -c --comp-alg rle -i archivo.txt -o archivo.txt.rle

# Descomprimir un archivo
./gsea -d --comp-alg rle -i archivo.txt.rle -o archivo_descomprimido.txt

# Mostrar ayuda
./gsea --help
```

## Algoritmo RLE
El algoritmo Run-Length Encoding funciona contando secuencias consecutivas del mismo carácter y las codifica como [carácter][contador].

**Ejemplo:**
- Entrada: `AAAABBBCCDDDD`
- Salida: `A4B3C2D4` (35.71% de reducción)

## Estructura del Proyecto
```
gsea/
├── src/
│   ├── main.c              # Función principal
│   ├── args.c              # Parser de argumentos
│   ├── file_manager.c      # Manejo de archivos con llamadas al sistema
│   └── compression.c       # Implementación del algoritmo RLE
├── include/
│   ├── args.h
│   ├── file_manager.h
│   └── compression.h
├── Makefile
└── README.md
```

## Estado del Proyecto
- ✅ **Punto 1**: Estructura base del proyecto
- ✅ **Punto 2**: Parser de argumentos de línea de comandos
- ✅ **Punto 3**: Makefile básico
- ✅ **Punto 4**: Funciones de manejo de archivos con llamadas al sistema
- ✅ **Punto 5**: Algoritmo de compresión RLE implementado
- ✅ **Punto 6**: Documentación en español

## Próximas Versiones
- ⏳ **Encriptación**: Algoritmo Vigenère
- ⏳ **Concurrencia**: Procesamiento paralelo de directorios
- ⏳ **Optimización**: Mejoras de rendimiento
- ⏳ **Documentación**: Documento técnico completo
