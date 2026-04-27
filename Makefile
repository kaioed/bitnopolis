CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -I./include -I./test/Unity -g
CFLAGS = -std=c99 -Wall -Wextra -DUNITY_INCLUDE_DOUBLE -I./include -I./test/Unity -g

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
TEST_DIR = test

MAIN_SRC = main.c
MODULES_SRC = $(wildcard $(SRC_DIR)/*.c)
MODULES_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(MODULES_SRC))
MAIN_OBJ = $(OBJ_DIR)/main.o

EXEC = ted
UNITY_SRC = $(TEST_DIR)/Unity/unity.c
UNITY_OBJ = $(OBJ_DIR)/unity.o

all: $(OBJ_DIR) $(EXEC)

$(EXEC): $(MODULES_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(UNITY_OBJ): $(UNITY_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

test: test_hash test_quadra test_geo test_arg test_casa test_pm test_qry

test_hash: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_hash.c $(UNITY_OBJ) $(SRC_DIR)/hash_extensivel.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_quadra: $(MODULES_OBJ) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_quadra.c $(UNITY_OBJ) $(MODULES_OBJ) -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@
	
test_geo: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_geo.c $(UNITY_OBJ) $(SRC_DIR)/geo.c $(SRC_DIR)/quadra.c $(SRC_DIR)/hash_extensivel.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_arg: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_arg.c $(UNITY_OBJ) $(SRC_DIR)/arg.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_casa: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_casa.c $(UNITY_OBJ) $(SRC_DIR)/casa.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_pm: $(MODULES_OBJ) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_pm.c $(UNITY_OBJ) $(MODULES_OBJ) -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_qry: $(MODULES_OBJ) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_qry.c $(UNITY_OBJ) $(MODULES_OBJ) -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

clean_tests:
	rm -f $(OBJ_DIR)/test_* *.svg

.PHONY: all clean clean_tests test test_hash test_quadra test_geo test_arg test_casa test_pm test_qry