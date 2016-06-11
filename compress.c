#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "function_compress.h"
#define INT uint32_t
#define CHAR uint8_t

int main(int argc, char *argv[3])
{
	FILE *input, *compress, *output, *rasc;
	CHAR *sb;

	sb = (CHAR*) calloc(32768,sizeof(CHAR));

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "wb");
	rasc = fopen("rascunho_compress.txt", "wb");

	if(input == NULL)
	{
		printf("Erro: O arquivo não foi encontrado.\n");
		return 1;
	}

	compress = fopen("didatico_compress.txt", "wb");

	fazLeitura(input, compress, sb, 0);

	fclose(compress);

	compress = fopen("didatico_compress.txt", "rb");

	escritaEmBinario(compress, rasc);

	fclose(rasc);

	rasc = fopen("rascunho_compress.txt", "rb");

	formataBinario(rasc, output);

	free(sb);
	fclose(rasc);
	fclose(input);
	fclose(output);
	fclose(compress);
	remove("rascunho_compress.txt");
	remove("didatico_compress.txt");

	return 0;
}
