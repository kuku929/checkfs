CC=clang

all: src/checkfs.c src/checkfs.h
	$(CC) -o checkfs src/checkfs.c
