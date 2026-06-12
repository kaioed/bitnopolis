CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -DUNITY_INCLUDE_DOUBLE -I../include -I../test/Unity -g

OBJ_DIR = obj
TEST_DIR = ../test

MAIN_SRC = ../main.c
MODULES_SRC = $(filter-out main.c, $(wildcard *.c))
MODULES_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(MODULES_SRC))
MAIN_OBJ = $(OBJ_DIR)/main.o

EXEC = ted
UNITY_SRC = $(TEST_DIR)/Unity/unity.c
UNITY_OBJ = $(OBJ_DIR)/unity.o

ifeq ($(OS),Windows_NT)
MKDIR_P = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
else
MKDIR_P = mkdir -p $(OBJ_DIR)
endif

all: $(EXEC)

$(EXEC): $(MODULES_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(UNITY_OBJ): $(UNITY_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR_P)

test: test_hash test_quadra test_geo test_arg test_casa test_pm test_qry

test_hash: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_hash.c $(UNITY_OBJ) hash_extensivel.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_quadra: $(MODULES_OBJ) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_quadra.c $(UNITY_OBJ) $(MODULES_OBJ) -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_geo: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_geo.c $(UNITY_OBJ) geo.c quadra.c hash_extensivel.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_arg: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_arg.c $(UNITY_OBJ) arg.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_casa: $(OBJ_DIR) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_casa.c $(UNITY_OBJ) casa.c -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_pm: $(MODULES_OBJ) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_pm.c $(UNITY_OBJ) $(MODULES_OBJ) -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

test_qry: $(MODULES_OBJ) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/teste_qry.c $(UNITY_OBJ) $(MODULES_OBJ) -o $(OBJ_DIR)/$@
	./$(OBJ_DIR)/$@

ifeq ($(OS),Windows_NT)
clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(EXEC) del /f /q $(EXEC)
	if exist $(EXEC).exe del /f /q $(EXEC).exe

clean_tests:
	if exist $(OBJ_DIR)\test_* del /f /q $(OBJ_DIR)\test_*
	if exist *.svg del /f /q *.svg
else
clean:
	rm -rf $(OBJ_DIR) $(EXEC) $(EXEC).exe

clean_tests:
	rm -f $(OBJ_DIR)/test_* *.svg
endif

.PHONY: all clean clean_tests test test_hash test_quadra test_geo test_arg test_casa test_pm test_qry
