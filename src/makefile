CC = gcc
# O -I../include diz ao compilador para procurar os .h uma pasta para trás
CFLAGS = -std=c99 -Wall -Wextra -DUNITY_INCLUDE_DOUBLE -I../include -I../test/Unity -g

# Pega todos os arquivos .c dentro da pasta src atual e o main.c na pasta raiz (../)
SRCS = $(wildcard *.c)

# Converte a lista de .c para .o correspondentes
OBJS = $(SRCS:.c=.o)

# Nome do executável exigido pelo professor
EXEC = ted

# Regra padrão
all: $(EXEC)

# Regra 'ted' exata que o script do professor está procurando
ted: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Regra genérica para compilar os arquivos .c em objetos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpeza (caso o script do professor rode 'make clean')
clean:
	rm -f $(OBJS) $(EXEC)