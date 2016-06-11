#ifndef function_decompress
#define function_decompress
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#define INT uint32_t
#define CHAR uint8_t


void transformaPonteiroR_Offset(CHAR *str, FILE *rasc);

void transformaPonteiroComp(CHAR *str, FILE *rasc);

void transformaLiteral(CHAR *str, FILE *rasc);

void transformaEmNumeros(FILE *input, FILE *rasc);

void imprimePonteiroNormal(int comp, int r_offset, FILE *output);

void imprimePonteiroRepete(int comp, int r_offset, FILE* output);

void montaPonteiro(int comp, int r_offset, FILE* output);

void descomprime(FILE *rasc, FILE *output);

#endif
