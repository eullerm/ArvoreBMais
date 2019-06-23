#ifndef PIZZA_H
#define PIZZA_H

#include <stdio.h>

typedef struct pizza {
	int cod;
	char nome[50];
	char categoria[20];
	float preco;
} TPizza;

// Imprime pizza
void imprime_pizza(TPizza *p);

// Cria pizza. Lembrar de usar free(pizza)
TPizza *pizza(int cod, char *nome, char *categoria, float preco);

// Salva pizza no arquivo out, na posicao atual do cursor
void salva_pizza(TPizza *p, FILE *out);

// Le uma pizza do arquivo in na posicao atual do cursor
// Retorna um ponteiro para pizza lida do arquivo
TPizza *le_pizza(FILE *in);

// Compara duas pizzas
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_pizza(TPizza *p1, TPizza *p2);

// Retorna tamanho do registro que representa a pizza em bytes
int tamanho_pizza_bytes();

#endif