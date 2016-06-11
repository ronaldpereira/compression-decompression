#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "function_decompress.h"
#define INT uint32_t
#define CHAR uint8_t

int main(int argc, char *argv[3])
{
	FILE *input, *output, *rasc;

	input = fopen(argv[1], "rb");
	output = fopen(argv[2], "w+r");
	rasc = fopen("rascunho_decompress.txt", "w");

	transformaEmNumeros(input, rasc);

	fclose(rasc);

	rasc = fopen("rascunho_decompress.txt", "r");

	descomprime(rasc, output);

	fclose(input);
	fclose(output);
	fclose(rasc);
	remove("rascunho_decompress.txt");

	return 0;
}
