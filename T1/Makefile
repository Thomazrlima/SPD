# Makefile para o programa de multiplicação de matrizes

CC = gcc                    # Compilador
CFLAGS = -Wall -O2           # Flags de compilação
LDFLAGS =                    # Flags de linkedição
EXEC = programa              # Nome do executável

# Arquivos de origem
SRCS = main.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# Regra padrão para compilar o programa
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Regra para compilar arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos objeto e executáveis
clean:
	rm -f $(OBJS) $(EXEC)

# Regra para gerar um arquivo de execução, incluindo a limpeza
rebuild: clean $(EXEC)

# Regra para testar o programa
run: $(EXEC)
	./$(EXEC) entradaA.txt entradaB.txt arquivo_saida.txt
