#ifndef LISTA_NOS_FOLHAS_H
#define LISTA_NOS_FOLHAS_H

#include "no_folha.h"

typedef struct ListaNosFolhas {
	TNoFolha **lista;
	int qtd;
} TListaNosFolhas;

// Imprime nohs folhas
void imprime_nos_folhas(int d, TListaNosFolhas *l);

// Cria lista de nohs folhas. Lembrar de usar libera_nos_folhas(lista_nos)
TListaNosFolhas *cria_nos_folhas(int d, int qtd, ...);

// Salva lista de nos folhas no arquivo nome_arquivo
void salva_nos_folhas(int d, char *nome_arquivo, TListaNosFolhas *l);

// Le lista de nos folhas do arquivo nome_arquivo
TListaNosFolhas *le_nos_folhas(int d, char *nome_arquivo);

// Compara duas listas de nohs folhas
// Retorna 1 se os nohs das duas listas forem iguais
// e 0 caso contrario
int cmp_nos_folhas(int d, TListaNosFolhas *n1, TListaNosFolhas *n2);

// Desaloca lista de nohs folhas
void libera_nos_folhas(int d, TListaNosFolhas *l);

#endif