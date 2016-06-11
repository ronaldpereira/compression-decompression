#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "function_decompress.h"
#define INT uint32_t
#define CHAR uint8_t

/**
 * Representação stdint:
 * uint8_t --> unsigned char
 * int32_t --> signed int
 * uint32_t --> unsigned int
 *
 * Tamanho:
 * 3 <= Comprimento <= 258 (2⁸)
 * 1 <= r_offset <= 32.768 (2¹⁵)
 *
 * Saída em binário:
 * pos[0] = 0 para literal, 1 para ponteiro (1 bit)
 * pos[1] ao pos[8] = comprimento em binário (8 bits)
 * pos[9] ao [23] = r_offset em binário (15 bits)
 */

void transformaPonteiroR_Offset(CHAR *str, FILE *rasc)
{
	int num = 1, i;

	for(i = 0; i < 15; i++)
	{
		if(str[14-i] == '1')
			num += pow(2,i);
	}
	fprintf(rasc, "%d>", num);
}

 void transformaPonteiroComp(CHAR *str, FILE *rasc)
 {
 	int num = 3, i;

 	for(i = 0; i < 8; i++)
 	{
 		if(str[7-i] == '1')
 			num += pow(2,i);
 	}
	fprintf(rasc, "<%d,", num);
 }

void transformaLiteral(CHAR *str, FILE *rasc)
{
	int num = 0, i;

	for(i = 0; i < 8; i++)
	{
		if(str[7-i] == '1')
		{
			num += pow(2,i);
		}
	}
	fprintf(rasc, "%c", num);
}

void transformaEmNumeros(FILE *input, FILE *rasc)
{
	CHAR c, *str;
	INT i;

	str = (CHAR*) calloc(15,sizeof(CHAR));

	while(!feof(input))
	{
		c = fgetc(input);

		if(c == '0')
		{
			for(i = 0; i < 8;)
			{
				c = fgetc(input);
				str[i] = c;
				if(c != ' ')
					i++;
			}
			str[i] = '\0';

			transformaLiteral(str, rasc);
		}

		else if(c == '1')
		{
			for(i = 0; i < 8;)
			{
				c = fgetc(input);
				str[i] = c;
				if(c != ' ')
					i++;
			}
			str[i] = '\0';

			transformaPonteiroComp(str, rasc);

			for(i = 0; i < 15;)
			{
				c = fgetc(input);
				str[i] = c;
				if(c != ' ')
					i++;
			}
			str[i] = '\0';

			transformaPonteiroR_Offset(str, rasc);
		}
	}

	free(str);
}

void imprimePonteiroNormal(int comp, int r_offset, FILE *output)
{
	int i;
	char c;

	for(i = comp; i > 0; i--)
	{
		fseek(output, -(r_offset), SEEK_END);
		c = fgetc(output);
		fseek(output, 0, SEEK_END);
		fprintf(output, "%c", c);
	}

}

void imprimePonteiroRepete(int comp, int r_offset, FILE* output)
{
	int i, j;
	int aux = (comp / r_offset);
	char c;

	for(i = 0; i < aux; i++)
	{
		for(j = (comp/aux); j > 0; j--)
		{
			fseek(output, -(r_offset), SEEK_END);
			c = fgetc(output);
			fseek(output, 0, SEEK_END);
			fprintf(output, "%c", c);
		}
	}
}

void montaPonteiro(int comp, int r_offset, FILE* output)
{
	if(comp <= r_offset)
		imprimePonteiroNormal(comp, r_offset, output);

	else if(comp > r_offset)
		imprimePonteiroRepete(comp, r_offset, output);

}

void descomprime(FILE *rasc, FILE *output)
{
	char c, *str;
	INT i;
	int comp, r_offset;

	str = (char*) calloc(10,sizeof(char));

	while(!feof(rasc))
	{
		c = fgetc(rasc);

		if(c != '<')
		{
			if((int)c != -1) // Sinal para o EOF não deve ser impresso
				fprintf(output, "%c", c);
		}

		else
		{
			c = fgetc(rasc);
			for(i = 0; c != ','; i++)
			{
				str[i] = c;
				c = fgetc(rasc);
			}
			str[i] = '\0';

			comp = atoi(str);

			c = fgetc(rasc);

			for(i = 0; c != '>'; i++)
			{
				str[i] = c;
				c = fgetc(rasc);
			}
			str[i] = '\0';

			r_offset = atoi(str);

			montaPonteiro(comp, r_offset, output);
		}
	}
}
