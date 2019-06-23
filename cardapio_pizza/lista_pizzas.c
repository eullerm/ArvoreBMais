#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdarg.h>

#include "lista_pizzas.h"


void imprime_pizzas(TListaPizzas *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		imprime_pizza(l->lista[i]);
	}
}

TListaPizzas *cria_pizzas(int qtd, ...)
{
	va_list ap;
	TListaPizzas *l = (TListaPizzas *)  malloc(sizeof(TListaPizzas));
	l->qtd = qtd;
	l->lista = (TPizza **) malloc(sizeof(TPizza *) * (qtd));
	int i;
	va_start(ap, qtd);
	for (i = 0; i < qtd; i++) {
        l->lista[i] = va_arg(ap, TPizza *);
    }
    va_end(ap);
    return l;
}

void salva_pizzas(char *nome_arquivo, TListaPizzas *l)
{
	FILE *out = fopen(nome_arquivo, "wb");
	int i;
	for (i = 0; i < l->qtd; i++) {
		salva_pizza(l->lista[i], out);
	}
	fclose(out);
}

TListaPizzas * le_pizzas(char *nome_arquivo)
{
	int qtd = 0;
	TListaPizzas *l = (TListaPizzas *)  malloc(sizeof(TListaPizzas));
	FILE *in = fopen(nome_arquivo, "rb");
	if (in != NULL) {
		TPizza *pizza = NULL;
		while((pizza = le_pizza(in)) != NULL) {
			qtd += 1;
			free(pizza);
		}
		fseek(in, 0, SEEK_SET);
		l->qtd = qtd;
		l->lista = (TPizza **) malloc(sizeof(TPizza *) * (qtd));
		qtd = 0;
		while((pizza = le_pizza(in)) != NULL) {
			l->lista[qtd++] = pizza;
		}
		fclose(in);
	} else {
		l->qtd = 0;
		l->lista = NULL;
	}
	return l;
}

int cmp_pizzas(TListaPizzas *l1, TListaPizzas *l2)
{
	if (l1->qtd != l2->qtd) {
		return 0;
	}
	int i;
	for (i = 0; i < l1->qtd; i++) {
		if (!cmp_pizza(l1->lista[i], l2->lista[i])) {
			return 0;
		}
	}
	return 1;
}

void libera_pizzas(TListaPizzas *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		free(l->lista[i]);
	}
	free(l->lista);
	free(l);
}
