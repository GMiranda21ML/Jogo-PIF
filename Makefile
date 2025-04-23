# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib
INCLUDE_DIR = include

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/*.c)
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Nome do executável
TARGET = $(BIN_DIR)/game

# Bibliotecas
LIBS = -lraylib -lm -ldl -lGL -lpthread -lrt -lX11

# Regras
.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
