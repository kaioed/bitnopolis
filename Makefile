
CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -I./include -g

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
TEST_DIR = tests

# Arquivos do programa principal
MAIN_SRC = $(SRC_DIR)/main.c
MODULES_SRC = $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.c))
MODULES_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(MODULES_SRC))
MAIN_OBJ = $(OBJ_DIR)/main.o


EXEC = ted

UNITY_SRC = $(TEST_DIR)/unity.c
TEST_SRCS = $(wildcard $(TEST_DIR)/test_*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRCS))
UNITY_OBJ = $(OBJ_DIR)/unity.o

# Target padrão
all: $(EXEC)

# Criação do executável principal
$(EXEC): $(MODULES_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilação dos objetos em src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilação dos objetos de teste
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I./$(TEST_DIR) -c $< -o $@

# Cria o diretório de objetos se não existir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Target obrigatório para compilar e executar os testes unitários
test: $(MODULES_OBJ) $(UNITY_OBJ) $(TEST_OBJS)
	$(CC) $(CFLAGS) -I./$(TEST_DIR) -o run_tests $^
	./run_tests

# Limpeza dos arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(EXEC) run_tests

.PHONY: all clean test