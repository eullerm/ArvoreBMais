#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdarg.h>

#include "lista_nos_folhas.h"


void imprime_nos_folhas(int d, TListaNosFolhas *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		imprime_no_folha(d, l->lista[i]);
	}
}

TListaNosFolhas *cria_nos_folhas(int d, int qtd, ...)
{
	va_list ap;
	TListaNosFolhas *lc = (TListaNosFolhas *)  malloc(sizeof(TListaNosFolhas));
	lc->qtd = qtd;
	lc->lista = (TNoFolha **) malloc(sizeof(TNoFolha *) * (qtd));
	int i;
	va_start(ap, qtd);
	for (i = 0; i < qtd; i++) {
        lc->lista[i] = va_arg(ap, TNoFolha *);
    }
    va_end(ap);
    return lc;
}

void salva_nos_folhas(int d, char *nome_arquivo, TListaNosFolhas *l)
{
	FILE *out = fopen(nome_arquivo, "wb");
	int i;
	for (i = 0; i < l->qtd; i++) {
		salva_no_folha(d, l->lista[i], out);
	}
	fclose(out);
}

TListaNosFolhas *le_nos_folhas(int d, char *nome_arquivo)
{
	int qtd = 0;
	TListaNosFolhas *lc = (TListaNosFolhas *)  malloc(sizeof(TListaNosFolhas));
	FILE *in = fopen(nome_arquivo, "rb");
	if (in != NULL) {
		TNoFolha *no = NULL;
		while((no = le_no_folha(d, in)) != NULL) {
			qtd += 1;
			libera_no_folha(d, no);
		}
		fseek(in, 0, SEEK_SET);
		lc->qtd = qtd;
		lc->lista = (TNoFolha **) malloc(sizeof(TNoFolha *) * (qtd));
		qtd = 0;
		while((no = le_no_folha(d, in)) != NULL) {
			lc->lista[qtd++] = no;
		}
		fclose(in);
	} else {
		lc->qtd = 0;
		lc->lista = NULL;
	}
	return lc;
}

int cmp_nos_folhas(int d, TListaNosFolhas *n1, TListaNosFolhas *n2)
{
	if (n1->qtd != n2->qtd) {
		return 0;
	}
	int i;
	for (i = 0; i < n1->qtd; i++) {
		if (!cmp_no_folha(d, n1->lista[i], n2->lista[i])) {
			return 0;
		}
	}
	return 1;
}

void libera_nos_folhas(int d, TListaNosFolhas *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		libera_no_folha(d, l->lista[i]);
	}
	free(l->lista);
	free(l);
}
