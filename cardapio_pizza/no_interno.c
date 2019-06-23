#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "no_interno.h"


void imprime_no_interno(int d, TNoInterno *no)
{
	int i;
	printf("%d, %d, %d (", no->m, no->pont_pai, no->aponta_folha);
	for (i = 0; i < 2 * d; i++) {
		printf("%d, ", no->p[i]);
	}
	printf("%d), (", no->p[i]);
	for (i = 0; i < 2 * d - 1; i++) {
		printf("%d, ", no->chaves[i]);
	}
	printf("%d)\n", no->chaves[i]);
}

TNoInterno *no_interno(int d, int m, int pont_pai, int aponta_folha)
{
	int i;
	TNoInterno *no = (TNoInterno *) malloc(sizeof(TNoInterno));
	if (no) memset(no, 0, sizeof(TNoInterno));
	no->m = m;
	no->pont_pai = pont_pai;
	no->aponta_folha = aponta_folha;
	no->p = (int *) malloc(sizeof(int) * (2 * d + 1));
	no->chaves = (int *) malloc(sizeof(int) * (2 * d));
	for (i = 0; i < 2 * d; i++) {
		no->p[i] = -1;
		no->chaves[i] = -1;
	}
	no->p[2 * d] = -1;
	return no;
}

TNoInterno *no_interno_vazio(int d)
{
	return no_interno(d, 0, -1, 0);
}

TNoInterno *cria_no_interno(int d, int m, int pont_pai, int aponta_folha, int size, ...)
{
	TNoInterno *n = no_interno(d, m, pont_pai, aponta_folha);
	int i;
	va_list ap;
	va_start(ap, size);
	for (i = 0; i < n->m + 1; i++) {
		n->p[i] = va_arg(ap, int);
	}
	for (i = 0; i < n->m; i++) {
		n->chaves[i] = va_arg(ap, int);
	}
	va_end(ap);
	return n;
}

void salva_no_interno(int d, TNoInterno *no, FILE *out)
{
	int i;
	fwrite(&no->m, sizeof(int), 1, out);
	fwrite(&no->pont_pai, sizeof(int), 1, out);
	fwrite(&no->aponta_folha, sizeof(int), 1, out);
	//garantidamente, sempre havera pelo menos 1 chave no noh
    //portanto, p0 sempre vai existir
	fwrite(&no->p[0], sizeof(int), 1, out);

	for (i = 0; i < 2 * d; i++) {
		fwrite(&no->chaves[i], sizeof(int), 1, out);
		fwrite(&no->p[i + 1], sizeof(int), 1, out);
	}
}

TNoInterno *le_no_interno(int d, FILE *in)
{
	int i;
	TNoInterno *no = (TNoInterno *) malloc(sizeof(TNoInterno));
	if (0 >= fread(&no->m, sizeof(int), 1, in)) {
		free(no);
		return NULL;
	}
	fread(&no->pont_pai, sizeof(int), 1, in);
	fread(&no->aponta_folha, sizeof(int), 1, in);
	no->p = (int *) malloc(sizeof(int) * (2 * d + 1));
	no->chaves = (int *) malloc(sizeof(int) * (2 * d));

	fread(&no->p[0], sizeof(int), 1, in);
	for (i = 0; i < 2 * d; i++) {
		fread(&no->chaves[i], sizeof(int), 1, in);
		fread(&no->p[i + 1], sizeof(int), 1, in);
	}
	return no;
}

int cmp_no_interno(int d, TNoInterno *n1, TNoInterno *n2)
{
	int i;
	if (n1 == NULL) {
		return (n2 == NULL);
	}
	if (n1->m != n2->m) {
		return 0;
	}
	if (n1->pont_pai != n2->pont_pai) {
		return 0;
	}
	if (n1->aponta_folha != n2->aponta_folha) {
		return 0;
	}
	for (i = 0; i < n1->m+1; i++) {
		if (n1->p[i] != n2->p[i]) {
			return 0;
		}
	}
	for (i = 0; i < n1->m; i++) {
		if (n1->chaves[i] != n2->chaves[i]) {
			return 0;
		}
	}
	return 1;
}

int tamanho_no_interno(int d)
{
	return sizeof(int) + // m
		sizeof(int) + // pont_pai
		sizeof(int) + // aponta_folha
		sizeof(int) * (2 * d + 1) + // p
		sizeof(int) * (2 * d); // chaves
}

void libera_no_interno(TNoInterno *no)
{
	free(no->p);
	free(no->chaves);
	free(no);
}