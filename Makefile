CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2
OBJS = main.o tabuleiro.o jogadores.o

all: campo_minado

campo_minado: $(OBJS)
	$(CC) $(CFLAGS) -o campo_minado $(OBJS)

run:
	./campo_minado

clean:
	rm -f *.o campo_minado
