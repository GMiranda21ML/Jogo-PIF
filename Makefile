# Compilador
CC = gcc

# Diretórios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Nome do executável
TARGET = game

# Flags de compilação
CFLAGS = -Wall -I$(INC_DIR) -O2
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL  # Ajuste se necessário (ex: no macOS -framework OpenGL -framework Cocoa)

# Regra principal
all: $(BUILD_DIR) $(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilação dos objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Criar diretório build se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpeza dos arquivos gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Recompilar tudo
rebuild: clean all
