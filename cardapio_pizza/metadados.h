#ifndef METADADOS_H
#define METADADOS_H

#include <stdio.h>

typedef struct Metadados {
    //ordem da arvore
    int d;
	// ponteiro para a raiz da arvore
	int pont_raiz;
	// flag que diz se a raiz eh uma folha (nesse caso ela esta no arquivo de dados)
	int raiz_folha;
	// ponteiro para o proximo noh interno livre
	int pont_prox_no_interno_livre;
	// ponteiro para o proximo noh folha livre
	int pont_prox_no_folha_livre;
} TMetadados;

// Imprime metadados
void imprime_metadados(TMetadados *metadados);

// Cria metadados. Lembrar de usar free(metadados)
TMetadados *metadados(int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre);

// Cria metadados vazio. Lembrar de usar free(metadados)
TMetadados *metadados_vazio();

// Salva metadados no arquivo out, na posicao atual do cursor
void salva_metadados(TMetadados *metadados, FILE *out);

// Salva metadados no arquivo nome_arquivo
void salva_arq_metadados(char *nome_arquivo, TMetadados *metadados);

// Le um metadados do arquivo in na posicao atual do cursor
// Retorna um ponteiro para metadados lido do arquivo
TMetadados *le_metadados(FILE *in);

// Le metadados do arquivo nome_arquivo
TMetadados *le_arq_metadados(char *nome_arquivo);

// Compara dois metadados
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_metadados(int d, TMetadados *m1, TMetadados *m2);

// Retorna tamanho do metadados em bytes
int tamanho_metadados();

#endif