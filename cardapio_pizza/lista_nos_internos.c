#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdarg.h>

#include "lista_nos_internos.h"


void imprime_nos_internos(int d, TListaNosInternos *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		imprime_no_interno(d, l->lista[i]);
	}
}

TListaNosInternos *cria_nos_internos(int d, int qtd, ...)
{
	va_list ap;
	TListaNosInternos *l = (TListaNosInternos *)  malloc(sizeof(TListaNosInternos));
	l->qtd = qtd;
	l->lista = (TNoInterno **) malloc(sizeof(TNoInterno *) * (qtd));
	int i;
	va_start(ap, qtd);
	for (i = 0; i < qtd; i++) {
        l->lista[i] = va_arg(ap, TNoInterno *);
    }
    va_end(ap);
    return l;
}

void salva_nos_internos(int d, char *nome_arquivo, TListaNosInternos *l)
{
	FILE *out = fopen(nome_arquivo, "wb");
	int i;
	for (i = 0; i < l->qtd; i++) {
		salva_no_interno(d, l->lista[i], out);
	}
	fclose(out);
}

TListaNosInternos * le_nos_internos(int d, char *nome_arquivo)
{
	int qtd = 0;
	TListaNosInternos *l = (TListaNosInternos *)  malloc(sizeof(TListaNosInternos));
	FILE *in = fopen(nome_arquivo, "rb");
	if (in != NULL) {
		TNoInterno *no = NULL;
		while((no = le_no_interno(d, in)) != NULL) {
			qtd += 1;
			libera_no_interno(no);
		}
		fseek(in, 0, SEEK_SET);
		l->qtd = qtd;
		l->lista = (TNoInterno **) malloc(sizeof(TNoInterno *) * (qtd));
		qtd = 0;
		while((no = le_no_interno(d, in)) != NULL) {
			l->lista[qtd++] = no;
		}
		fclose(in);
	} else {
		l->qtd = 0;
		l->lista = NULL;
	}
	return l;
}

int cmp_nos_internos(int d, TListaNosInternos *n1, TListaNosInternos *n2)
{
	if (n1->qtd != n2->qtd) {
		return 0;
	}
	int i;
	for (i = 0; i < n1->qtd; i++) {
		if (!cmp_no_interno(d, n1->lista[i], n2->lista[i])) {
			return 0;
		}
	}
	return 1;
}

void libera_nos_internos(TListaNosInternos *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		libera_no_interno(l->lista[i]);
	}
	free(l->lista);
	free(l);
}
