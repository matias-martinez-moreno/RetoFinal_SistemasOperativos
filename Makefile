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

# Limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	@echo "Limpieza completada"

# Instalar el programa (opcional)
install: $(TARGET)
	@echo "Instalando $(TARGET) en /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Instalación completada"

# Desinstalar el programa
uninstall:
	@echo "Desinstalando $(TARGET)..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Desinstalación completada"

# Crear directorio de objetos si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Reglas phony
.PHONY: all clean install uninstall

# Información de ayuda
help:
	@echo "Makefile para GSEA"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make          - Compilar el proyecto"
	@echo "  make clean    - Limpiar archivos generados"
	@echo "  make install  - Instalar en /usr/local/bin"
	@echo "  make uninstall - Desinstalar"
	@echo "  make help     - Mostrar esta ayuda"
