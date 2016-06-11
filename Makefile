CC = gcc
CFLAGS = -Wall -Wextra -g -O0

all: compress.exe decompress.exe

compress.exe: compress.o function_compress.o
	$(CC) $(CFLAGS) compress.o function_compress.o -lm -o compress.exe

compress.o: compress.c function_compress.h
	$(CC) $(CFLAGS) -c compress.c

decompress.exe: decompress.o function_decompress.o
	$(CC) $(CFLAGS) decompress.o function_decompress.o -lm -o decompress.exe

decompress.o: decompress.c function_decompress.h
	$(CC) $(CFLAGS) -c decompress.c

function_compress.o: function_compress.c function_compress.h
	$(CC) $(CFLAGS) -c function_compress.c

function_decompress.o: function_decompress.c function_decompress.h
	$(CC) $(CFLAGS) -c function_decompress.c

clean:
	rm *.o decompress.exe compress.exe
