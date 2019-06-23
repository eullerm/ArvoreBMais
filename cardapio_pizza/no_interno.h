#ifndef NO_INTERNO_H
#define NO_INTERNO_H

#include <stdio.h>

typedef struct NoInterno {
	// m eh o numero total de chaves armazenados no noh
	int m;
	// ponteiro para o noh pai
	int pont_pai;
	// flag que diz se noh aponta pra folha ou nao
	int aponta_folha;
	// essa lista tera sempre m + 1 ponteiros
	int *p;
	// essa lista tera sempre m chaves
	int *chaves;
} TNoInterno;

// Imprime noh interno
void imprime_no_interno(int d, TNoInterno *no);

// Cria noh interno. Lembrar de usar libera_no_interno(no)
TNoInterno *no_interno(int d, int m, int pont_pai, int aponta_folha);

// Cria noh interno. Lembrar de usar libera_no_interno(no)
TNoInterno *no_interno_vazio(int d);

// Cria noh interno com dados. Lembrar de usar libera_no_interno(no)
// Size deve ser 2m + 1
TNoInterno *cria_no_interno(int d, int m, int pont_pai, int aponta_folha, int size, ...);

// Grava uma pagina (noh) no arquivo em disco
void salva_no_interno(int d, TNoInterno *no, FILE *out);

// Le uma pagina (noh) do disco
TNoInterno *le_no_interno(int d, FILE *in);

// Compara dois nohs internos
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_no_interno(int d, TNoInterno *n1, TNoInterno *n2);

// Retorna tamanho do noh interno em bytes
int tamanho_no_interno(int d);

// Libera noh interno
void libera_no_interno(TNoInterno *no);

#endif