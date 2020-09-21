CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3
DEP = Trie.h
OBJ = Boggle.o Trie.o

%.o: %.c ${DEP}
	${CC} -c -o $@ $< ${CFLAGS}

Boggle: ${OBJ}
	${CC} -o $@ $^ ${CFLAGS}

clean:
	rm -f Boggle *.o
