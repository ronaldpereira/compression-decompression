#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "function_compress.h"
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

int BMH(CHAR *orig, long tamorig, CHAR *trecho, long tamtrecho, INT pos)
{
	long i, j, k, d[257];

	for (j = 0; j <= 256 ; j++)
		d[j] = tamtrecho;

	for (j = 1; j < tamtrecho; j++)
		d[trecho[j-1]] = tamtrecho - j;

	i = tamtrecho;

	while (i <= tamorig)
	{
		k = i ;
		j = tamtrecho;

		while(orig[k-1] == trecho[j-1] && j > 0)
		{
			k--;
			j--;
		}

		if (j == 0 && k < pos)
		return k; // Retorna a posição na string orig do matching

		i += d[orig[i-1]];
	}
	return -1; // Não tem matching
}

void conversaoEmBinarioLiteral(INT p, FILE *rasc)
{
	int32_t i, aux;
	CHAR* s;

	s = (CHAR*) calloc(8,sizeof(CHAR));

	// max = 255 = 1111 1111
	for(i = 7; i >= 0; i--)
	{
		aux = p % 2;
		p /= 2;

		if(aux == 1)
			s[i] = '1';

		else
			s[i] = '0';
	}
	s[i] = '\0';
	fprintf(rasc, "%s", s);
	free(s);
}

void conversaoEmBinarioPonteiroComprimento(INT p, FILE* rasc)
{
	p -= 3;
	conversaoEmBinarioLiteral(p, rasc);
}

void conversaoEmBinarioPonteiroR_Offset(INT p, FILE* rasc)
{
	char *s;

	s = (char*) calloc(16,sizeof(char));

	p -= 1;

	int32_t i, aux;

	// max = 32767 bits = 0111 1111 1111 1111
	for(i = 14; i >= 0; i--)
	{
		aux = p % 2;
		p /= 2;

		if(aux == 1)
			s[i] = '1';

		else
			s[i] = '0';
	}
	fprintf(rasc, "%s", s);
	free(s);
}

void escritaEmBinario(FILE *tmp, FILE *rasc)
{
	CHAR c;
	INT p, count = 0, i;
	char *str;

	while(!feof(tmp))
	{
		str = (char*) calloc(9,sizeof(char));

		count++;

		c = fgetc(tmp);

		if(c == '<')
		{
			fprintf(rasc, "1");

			c = fgetc(tmp);
			for(i = 0; c != ','; i++)
			{
				str[i] = c;
				c = fgetc(tmp);
			}
			str[i+1] = '\0';

			p = atoi(str);
			conversaoEmBinarioPonteiroComprimento(p, rasc);

			c = fgetc(tmp);
			for(i = 0; c != '>'; i++)
			{
				str[i] = c;
				c = fgetc(tmp);
			}
			str[i+1] = '\0';

			p = atoi(str);
			conversaoEmBinarioPonteiroR_Offset(p, rasc);
		}

		else
		{
			p = (int)c;
			if(p != 255) // Exclui o char que indica EOF
			{
				fprintf(rasc, "0");
				conversaoEmBinarioLiteral(p, rasc);
			}
		}

		free(str);
	}
}

void matching(FILE *tmp, int32_t r_offset, INT comp)
{
	fprintf(tmp, "<%d,%d>", comp, r_offset);
}

void noMatching(FILE *tmp, CHAR *sb, INT pos)
{
	fprintf(tmp, "%c", sb[pos]);
}

void compression(FILE *tmp, CHAR *sb)
{
	INT lensb = 0, lencs = 0;
	INT i, j, k;
	int32_t pos, last;
	CHAR *cs;

	cs = (CHAR*) calloc(258,sizeof(CHAR));

	for(i = 0; sb[i] != '\0'; i++)
		lensb++; // equivalente a strlen(sb)

	i = 0;

	while(i < lensb)
	{
		if(i < 2)
			noMatching(tmp, sb, i);

		else
		{
			for(j = 0; j < 3 && i+j < lensb; j++)
				cs[j] = sb[i+j];
				cs[j] = '\0';
			lencs = 3;

			pos = BMH(sb, lensb, cs, lencs, i);

			if(pos == -1)
				noMatching(tmp, sb, i);

			if(pos != -1)
			{
				for(k = 0; pos != -1 && i+j+k <= lensb; k++)
				{
					last = pos;
					cs[j+k] = sb[i+j+k];
					lencs++;
					pos = BMH(sb, lensb, cs, lencs, i);
					if(pos == -1)
						matching(tmp, i-last, lencs-1);
				}

			for(j = 2; lencs - j > 0; j++)
				sb[i+lencs-j] = '>'; // Substituindo na string original por alguma coisa que não seja detectada como match depois

			i = i + lencs - 2;
			}
		}

	i++;
	}

	free(cs);
}

CHAR *doJumps(FILE *input, CHAR *sb)
{
	INT i;

	for(i = 0; i <= 32767; i++)
		sb[i] = sb[i+1];

	sb[i] = fgetc(input);

	return sb;
}

void fazLeitura(FILE *input, FILE *tmp, CHAR *sb, INT count)
{
	CHAR c;
	INT i;

	for(i = count; !feof(input) && i <= 32767; i++)
	{
		c = fgetc(input);
		if((INT)c != 255) // Retirando o char que indica o EOF
			sb[i] = c;
	}

	// Chama função que faz a compressão
	compression(tmp, sb);

	if(i > 32767)
	{
		sb = doJumps(input, sb);
		fazLeitura(input, tmp, sb, count);
	}
}

void formataBinario(FILE *rasc, FILE *out)
{
	char *str, c;
	int i, j;

	str = (char*) calloc(10,sizeof(char));

	while(!feof(rasc))
	{
		for(i = 0; i < 8 && !feof(rasc); i++)
		{
			c = fgetc(rasc);
			str[i] = c;
		}

		if(feof(rasc))
		{
			for(j = i-1; j < 8; j++)
				str[j] = '0';
		}

		else
			str[i] = ' ';

		fprintf(out, "%s", str);
	}
	free(str);
}
