#ifndef function_compress
#define function_compress
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#define INT uint32_t
#define CHAR uint8_t

int BMH(CHAR *orig, long tamorig, CHAR *trecho, long tamtrecho, INT pos);

void conversaoEmBinarioLiteral(INT p, FILE *rasc);

void conversaoEmBinarioPonteiroComprimento(INT p, FILE* rasc);

void conversaoEmBinarioPonteiroR_Offset(INT p, FILE* rasc);

void escritaEmBinario(FILE *tmp, FILE *rasc);

void matching(FILE *tmp, int32_t r_offset, INT comp);

void noMatching(FILE *tmp, CHAR *sb, INT pos);

void compression(FILE *tmp, CHAR *sb);

CHAR *doJumps(FILE *input, CHAR *sb);

void fazLeitura(FILE *input, FILE *tmp, CHAR *sb, INT count);

void formataBinario(FILE *rasc, FILE *out);



#endif
