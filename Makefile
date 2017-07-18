CC = gcc $(CFLAGS)
CFLAGS = -Wall -Wextra -pedantic -g
OBJECTS = banhammer.o aes.o hash.o ll.o lex.yy.o

.PHONY: all
all: banhammer

banhammer: $(OBJECTS)
	$(CC) $(OBJECTS) -lfl -o banhammer

aes.o: aes.c
	$(CC)  -c aes.c -o aes.o

hash.o: hash.c
	$(CC)  -c hash.c -o hash.o

banhammer.o: banhammer.c
	$(CC)  -c banhammer.c -o banhammer.o

ll.o: ll.c
	$(CC)  -c ll.c -o ll.o

lex.yy.o: lex.yy.c
	cc -c lex.yy.c

lex.yy.c: words.l
	flex words.l

.PHONY: clean
clean:
	rm -f $(OBJECTS) banhammer lex.yy.c
