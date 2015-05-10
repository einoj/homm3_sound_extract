CC = gcc
CFLAGS  = -Wall


all: src/extract.c
	$(CC) $(CFLAGS) src/extract.c -o extract

clean:
	rm extract
