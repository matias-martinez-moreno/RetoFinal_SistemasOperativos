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

# Regla principal - compila y mantiene ejecutable
all: $(TARGET)

# Regla para compilar y limpiar (para entrega)
build: $(TARGET)
	@echo "Compilación completada: $(TARGET)"
	@echo "Ejecutable listo para pruebas"

# Regla para compilar, probar y limpiar
test: $(TARGET)
	@echo "Ejecutando pruebas básicas..."
	@echo "ATCGATCGATCGATCGATCGATCGATCGATCG" > test_genetico.txt
	@./$(TARGET) -c --comp-alg rle -i test_genetico.txt -o test_genetico.txt.rle
	@./$(TARGET) -d --comp-alg rle -i test_genetico.txt.rle -o test_genetico_restaurado.txt
	@echo "Pruebas completadas exitosamente"
	@make clean-test
	@echo "Archivos temporales eliminados"

# Regla para compilar, probar y limpiar TODO (para entrega)
deliver: $(TARGET)
	@echo "Ejecutando pruebas completas..."
	@echo "ATCGATCGATCGATCGATCGATCGATCGATCG" > test_genetico.txt
	@./$(TARGET) -c --comp-alg rle -i test_genetico.txt -o test_genetico.txt.rle
	@./$(TARGET) -d --comp-alg rle -i test_genetico.txt.rle -o test_genetico_restaurado.txt
	@mkdir -p test_dir
	@echo "ATCGATCGATCGATCG" > test_dir/archivo1.txt
	@./$(TARGET) -c --comp-alg rle -i test_dir -o test_dir_comprimido
	@echo "Pruebas completadas exitosamente"
	@make clean
	@echo "Proyecto listo para entrega - solo archivos de código fuente"

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
	rm -rf test_dir* directorio_* datos_geneticos* 2>/dev/null || true
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
	@echo "  make          - Compilar el proyecto (mantiene ejecutable)"
	@echo "  make build    - Compilar y mostrar mensaje de éxito"
	@echo "  make test     - Compilar, probar y limpiar automáticamente"
	@echo "  make deliver  - Compilar, probar TODO y limpiar (para entrega)"
	@echo "  make clean    - Limpiar archivos generados y temporales"
	@echo "  make clean-test - Limpiar solo archivos de prueba"
	@echo "  make help     - Mostrar esta ayuda"