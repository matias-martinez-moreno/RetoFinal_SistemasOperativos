# Makefile para GSEA - Utilidad de Gestión Segura y Eficiente de Archivos

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LDFLAGS = -lpthread

# Directorios
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Archivos fuente
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/args.c $(SRC_DIR)/file_manager.c $(SRC_DIR)/compression.c $(SRC_DIR)/encryption.c $(SRC_DIR)/directory_processor.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Nombre del ejecutable
TARGET = gsea

# Archivos temporales a limpiar
TEMP_FILES = *.txt *.rle *.enc *.ce *.de *.ec *.du test_dir* directorio_* datos_geneticos*

# Regla principal
all: $(TARGET)

# Crear el ejecutable
$(TARGET): $(OBJECTS)
	@echo "Enlazando $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilación completada: $(TARGET)"

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Limpiar archivos generados y temporales
clean:
	@echo "Limpiando archivos generados..."
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	rm -f $(TEMP_FILES)
	rm -rf test_dir* directorio_* datos_geneticos*
	@echo "Limpieza completada"

# Limpiar solo archivos temporales de prueba
clean-test:
	@echo "Limpiando archivos de prueba..."
	rm -f $(TEMP_FILES)
	rm -rf test_dir* directorio_* datos_geneticos*
	@echo "Limpieza de pruebas completada"

# Crear directorio de objetos si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Reglas phony
.PHONY: all clean clean-test

# Información de ayuda
help:
	@echo "Makefile para GSEA"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make          - Compilar el proyecto"
	@echo "  make clean    - Limpiar archivos generados y temporales"
	@echo "  make clean-test - Limpiar solo archivos de prueba"
	@echo "  make help     - Mostrar esta ayuda"