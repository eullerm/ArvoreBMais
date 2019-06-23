#include "pizza.h"

typedef struct ListaPizzas {
	TPizza **lista;
	int qtd;
} TListaPizzas;

// Imprime pizzas
void imprime_pizzas(TListaPizzas *l);

// Cria lista de pizzas. Lembrar de usar libera_pizzas(lista_pizzas)
TListaPizzas *cria_pizzas(int qtd, ...);

// Salva lista de pizzas no arquivo nome_arquivo
void salva_pizzas(char *nome_arquivo, TListaPizzas *l);

// Le lista de pizzas do arquivo nome_arquivo
TListaPizzas *le_pizzas(char *nome_arquivo);

// Compara duas listas de pizzas
// Retorna 1 se as pizzas das duas listas forem iguais
// e 0 caso contrario
int cmp_pizzas(TListaPizzas *l1, TListaPizzas *l2);

// Desaloca lista de pizzas
void libera_pizzas(TListaPizzas *l);