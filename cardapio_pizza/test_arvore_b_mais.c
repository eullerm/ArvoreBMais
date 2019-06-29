#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define BEFORE_TEST before();

#define NOME_ARQUIVO_METADADOS "metadados.dat"
#define NOME_ARQUIVO_INDICE "indice.dat"
#define NOME_ARQUIVO_DADOS "pizzas.dat"
#define NOME_ARQUIVO_INICIAL "dados_iniciais.dat"
#define D 2

#include <limits.h>
#include <stdlib.h>

#include "ufftest.h"
#include "pizza.h"
#include "no_interno.h"
#include "no_folha.h"
#include "lista_pizzas.h"
#include "lista_nos_internos.h"
#include "lista_nos_folhas.h"
#include "metadados.h"
#include "arvore_b_mais.h"


void before() {
    remove(NOME_ARQUIVO_METADADOS);
    remove(NOME_ARQUIVO_INDICE);
    remove(NOME_ARQUIVO_DADOS);
}

//Gera arquivo binario com dados iniciais
void gera_arquivo_carga_inicial_1() {
    TListaPizzas *lista = cria_pizzas(4,
                                      pizza(10, "Calabresa", "Salgada", 30),
                                      pizza(13, "Portuguesa", "Salgada", 32),
                                      pizza(15, "Bianca", "Salgada", 30),
                                      pizza(26, "Frango", "Salgada", 31)
    );
    salva_pizzas(NOME_ARQUIVO_INICIAL, lista);
    libera_pizzas(lista);
}

//Gera arquivo binario com dados iniciais
void gera_arquivo_carga_inicial_2() {
    TListaPizzas *lista = cria_pizzas(6,
                                      pizza(10, "Calabresa", "Salgada", 30),
                                      pizza(13, "Portuguesa", "Salgada", 32),
                                      pizza(15, "Bianca", "Salgada", 30),
                                      pizza(25, "Banana", "Doce", 25),
                                      pizza(26, "Frango", "Salgada", 31),
                                      pizza(35, "Camarao", "Salgada", 35)
    );
    salva_pizzas(NOME_ARQUIVO_INICIAL, lista);
    libera_pizzas(lista);
}

//Gera arquivo binario com dados iniciais
void gera_arquivo_carga_inicial_3() {
    TListaPizzas *lista = cria_pizzas(50,
                                      pizza(10, "Calabresa", "Salgada", 30),
                                      pizza(13, "Portuguesa", "Salgada", 32),
                                      pizza(15, "Bianca", "Salgada", 30),
                                      pizza(18, "Estrogonoff com Cogumelos", "Especial", 40),
                                      pizza(20, "Lombinho", "Salgada", 32),
                                      pizza(21, "Prestigio", "Doce", 25),
                                      pizza(23, "Presunto Parma com Rucula", "Especial", 50),
                                      pizza(25, "Banana", "Doce", 25),
                                      pizza(26, "Frango", "Salgada", 31),
                                      pizza(28, "Havaiana", "Salgada", 33),
                                      pizza(31, "Banana com Nutela", "Doce Especial", 42),
                                      pizza(35, "Camarao", "Salgada", 35),
                                      pizza(37, "Chocolate com Morango", "Doce", 28),
                                      pizza(38, "Calabresa Acebolada", "Salgada", 30),
                                      pizza(39, "Frango com Catupiry", "Salgada", 35),
                                      pizza(40, "Camarao com Cogumelos", "Especial", 51),
                                      pizza(42, "Queijo Brie com Damasco", "Doce Especial", 51),
                                      pizza(45, "Quatro Queijos", "Salgada", 35),
                                      pizza(50, "Marguerita", "Salgada", 28),
                                      pizza(55, "Morango com Nutella", "Doce Especial", 45),
                                      pizza(60, "Presunto, Queijo e Tomate", "Salgada", 31),
                                      pizza(70, "Muzzarela", "Salgada", 28),
                                      pizza(77, "Atum", "Salgada", 29),
                                      pizza(79, "Aliche", "Salgada", 30),
                                      pizza(83, "Picadinho de Filet", "Salgada", 35),
                                      pizza(85, "Calabresa Picante", "Salgada", 31),
                                      pizza(87, "Lombinho com Catupiry", "Salgada", 37),
                                      pizza(90, "Brocolis com Alho", "Vegetariana", 28),
                                      pizza(93, "Rucula", "Vegetariana", 29),
                                      pizza(95, "Rucula com Tomate Seco", "Especial", 40),
                                      pizza(96, "Palmito", "Vegetariana", 37),
                                      pizza(98, "Caprese", "Vegetariana", 39),
                                      pizza(102, "Presunto Parma com Muzzarela de Bufala", "Especial", 57),
                                      pizza(103, "Presunto e Azeitonas", "Salgada", 35),
                                      pizza(110, "Calabresa com Catupiry", "Salgada", 32),
                                      pizza(111, "Linguica Artesanal com Cubos de Queijo Qualho", "Especial", 50),
                                      pizza(112, "Abobrinha", "Vegetariana", 29),
                                      pizza(115, "Alho e Oleo", "Vegetariana", 25),
                                      pizza(119, "Peperoni", "Salgada", 31),
                                      pizza(121, "Abobrinha, Beringela e Tomate Seco", "Vegetariana", 40),
                                      pizza(125, "Cogumelhos Especiais", "Especial", 57),
                                      pizza(132, "Peito de Peru", "Salgada", 24),
                                      pizza(133, "Coracao de Galinha", "Salgada", 30),
                                      pizza(135, "Milho com Bacon", "Salgada", 32),
                                      pizza(137, "Frango, Milho e Palmito", "Salgada", 27),
                                      pizza(140, "Camarao com Catupiry", "Salgada", 40),
                                      pizza(141, "Queijo Brie com Figo", "Doce Especial", 55),
                                      pizza(142, "Presunto e Azeitonas com Catupiry", "Salgada", 30),
                                      pizza(144, "Peperoni com Catupiry", "Salgada", 29),
                                      pizza(146, "Brocolis com Bacon", "Salgada", 29
                                      )
    );
    salva_pizzas(NOME_ARQUIVO_INICIAL, lista);
    libera_pizzas(lista);
}


// Arvore B+ de altura H = 1, com apenas 1 noh (raiz eh uma folha) que tem 2 registros
// Ver arquivo ArvoreTesteDOJO.pdf para o desenho da arvore
void monta_arvore_h1() {
    TMetadados *tabMetadados = metadados(D, 0, 1, 0, 1 * tamanho_no_folha(D));
    salva_arq_metadados(NOME_ARQUIVO_METADADOS, tabMetadados);

    //cria arquivo de indice vazio
    fclose(fopen(NOME_ARQUIVO_INDICE, "w"));

    TListaNosFolhas *tabDados = cria_nos_folhas(D, 1,
                                                cria_no_folha(D, -1, -1, 2,
                                                              pizza(10, "Calabresa", "Salgada", 30),
                                                              pizza(13, "Portuguesa", "Salgada", 32)));
    salva_nos_folhas(D, NOME_ARQUIVO_DADOS, tabDados);
    free(tabMetadados);
    libera_nos_folhas(D, tabDados);
}

// Arvore B+ de altura H = 1, com apenas 1 noh (raiz eh uma folha) que tem 4 registros
// Ver arquivo ArvoreTesteDOJO.pdf para o desenho da arvore
void monta_arvore_h1_cheia() {
    TMetadados *tabMetadados = metadados(D, 0, 1, 0, 1 * tamanho_no_folha(D));
    salva_arq_metadados(NOME_ARQUIVO_METADADOS, tabMetadados);

    //cria arquivo de indice vazio
    fclose(fopen(NOME_ARQUIVO_INDICE, "w"));

    TListaNosFolhas *tabDados = cria_nos_folhas(D, 1,
                                                cria_no_folha(D, -1, -1, 4,
                                                              pizza(10, "Calabresa", "Salgada", 30),
                                                              pizza(13, "Portuguesa", "Salgada", 32),
                                                              pizza(15, "Bianca", "Salgada", 30),
                                                              pizza(26, "Frango", "Salgada", 31)));
    salva_nos_folhas(D, NOME_ARQUIVO_DADOS, tabDados);
    free(tabMetadados);
    libera_nos_folhas(D, tabDados);
}

// Arvore B+ de altura H = 2, com raiz e mais 3 folhas
// Ordem de insercao das chaves na arvores: 10, 13, 15, 20, 25, 35, 37
// Ver arquivo ArvoreTesteDOJO.pdf para o desenho da arvore
void monta_arvore_h2() {
    TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
    salva_arq_metadados(NOME_ARQUIVO_METADADOS, tabMetadados);
    // Estrutura do No Interno: m, pont_pai, aponta_folha, 2m + 1, lista de ponteiros, lista de chaves
    // Atencao: os ponteiros sao absolutos
    TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                     cria_no_interno(D, 2, -1, 1, 2 * 2 + 1,
                                                                     0,
                                                                     1 * tamanho_no_folha(D),
                                                                     2 * tamanho_no_folha(D),
                                                                     15,
                                                                     25
                                                     )
    );
    salva_nos_internos(D, NOME_ARQUIVO_INDICE, tabIndice);

    // Estrutura do No Folha: pont_pai, pont_prox, m, registros de pizzas
    TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                              pizza(10, "Calabresa", "Salgada", 30),
                                                              pizza(13, "Portuguesa", "Salgada", 32)
                                                ),
                                                cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 2,
                                                              pizza(15, "Bianca", "Salgada", 30),
                                                              pizza(20, "Lombinho", "Salgada", 32)
                                                ),
                                                cria_no_folha(D, 0, -1, 3,
                                                              pizza(25, "Banana", "Doce", 25),
                                                              pizza(35, "Camarao", "Salgada", 35),
                                                              pizza(37, "Chocolate com Morango", "Doce", 28)
                                                )
    );
    salva_nos_folhas(D, NOME_ARQUIVO_DADOS, tabDados);
    free(tabMetadados);
    libera_nos_internos(tabIndice);
    libera_nos_folhas(D, tabDados);
}

// Arvore B+ de altura H = 2, com raiz e mais 3 folhas, 1 delas cheia
// Ordem de insercao das chaves na arvore: 10, 13, 15, 20, 25, 35, 37, 21, 23
// Ver arquivo ArvoreTesteDOJO.pdf para o desenho da arvore
void monta_arvore_h2_cheia() {
    TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
    salva_arq_metadados(NOME_ARQUIVO_METADADOS, tabMetadados);
    // Estrutura do No Interno: m, pont_pai, aponta_folha, 2m + 1, lista de ponteiros, lista de chaves
    // Atencao: os ponteiros sao absolutos
    TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                     cria_no_interno(D, 2, -1, 1, 2 * 2 + 1,
                                                                     0,
                                                                     1 * tamanho_no_folha(D),
                                                                     2 * tamanho_no_folha(D),
                                                                     15,
                                                                     25
                                                     )
    );
    salva_nos_internos(D, NOME_ARQUIVO_INDICE, tabIndice);

    // Estrutura do No Folha: pont_pai, pont_prox, m, registros de pizzas
    TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                              pizza(10, "Calabresa", "Salgada", 30),
                                                              pizza(13, "Portuguesa", "Salgada", 32)
                                                ),
                                                cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 4,
                                                              pizza(15, "Bianca", "Salgada", 30),
                                                              pizza(20, "Lombinho", "Salgada", 32),
                                                              pizza(21, "Prestigio", "Doce", 25),
                                                              pizza(23, "Presunto Parma com Rucula", "Especial", 50)
                                                ),
                                                cria_no_folha(D, 0, -1, 3,
                                                              pizza(25, "Banana", "Doce", 25),
                                                              pizza(35, "Camarao", "Salgada", 35),
                                                              pizza(37, "Chocolate com Morango", "Doce", 28)
                                                )
    );
    salva_nos_folhas(D, NOME_ARQUIVO_DADOS, tabDados);
    free(tabMetadados);
    libera_nos_internos(tabIndice);
    libera_nos_folhas(D, tabDados);
}

// Arvore B+ de altura H = 2, com raiz e mais 5 folhas, 1 delas cheia
// Ver arquivo ArvoreTesteDOJO.pdf para o desenho da arvore
void monta_arvore_h2_completa() {
    TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 5 * tamanho_no_folha(D));
    salva_arq_metadados(NOME_ARQUIVO_METADADOS, tabMetadados);
    // Estrutura do No Interno: m, pont_pai, aponta_folha, 2m + 1, lista de ponteiros, lista de chaves
    // Atencao: os ponteiros sao absolutos
    TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                     cria_no_interno(D, 4, -1, 1, 4 * 2 + 1,
                                                                     0,
                                                                     1 * tamanho_no_folha(D),
                                                                     2 * tamanho_no_folha(D),
                                                                     3 * tamanho_no_folha(D),
                                                                     4 * tamanho_no_folha(D),
                                                                     15,
                                                                     25,
                                                                     40,
                                                                     70
                                                     )
    );
    salva_nos_internos(D, NOME_ARQUIVO_INDICE, tabIndice);

    // Estrutura do No Folha: pont_pai, pont_prox, m, registros de pizzas
    TListaNosFolhas *tabDados = cria_nos_folhas(D, 5,
                                                cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                              pizza(10, "Calabresa", "Salgada", 30),
                                                              pizza(13, "Portuguesa", "Salgada", 32)
                                                ),
                                                cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 4,
                                                              pizza(15, "Bianca", "Salgada", 30),
                                                              pizza(20, "Lombinho", "Salgada", 32),
                                                              pizza(21, "Prestigio", "Doce", 25),
                                                              pizza(23, "Presunto Parma com Rucula", "Especial", 50)
                                                ),
                                                cria_no_folha(D, 0, 3 * tamanho_no_folha(D), 3,
                                                              pizza(25, "Banana", "Doce", 25),
                                                              pizza(35, "Camarao", "Salgada", 35),
                                                              pizza(37, "Chocolate com Morango", "Doce", 28)
                                                ),
                                                cria_no_folha(D, 0, 4 * tamanho_no_folha(D), 2,
                                                              pizza(40, "Camarao com Cogumelos", "Especial", 51),
                                                              pizza(45, "Quatro Queijos", "Salgada", 35)
                                                ),
                                                cria_no_folha(D, 0, -1, 2,
                                                              pizza(70, "Muzzarela", "Salgada", 28),
                                                              pizza(77, "Atum", "Salgada", 29)
                                                )
    );
    salva_nos_folhas(D, NOME_ARQUIVO_DADOS, tabDados);
    free(tabMetadados);
    libera_nos_internos(tabIndice);
    libera_nos_folhas(D, tabDados);
}

// Arvore B de altura H = 3, com raiz que aponta para 2 nohs internos, e mais 6 folhas
// Ver arquivo ArvoreTesteDOJO.pdf para o desenho da arvore
void monta_arvore_h3() {
    TMetadados *tabMetadados = metadados(D, 0, 0, 3 * tamanho_no_interno(D), 6 * tamanho_no_folha(D));
    salva_arq_metadados(NOME_ARQUIVO_METADADOS, tabMetadados);
    // Estrutura do No Interno: m, pont_pai, aponta_folha, 2m + 1, lista de ponteiros, lista de chaves
    // Atencao: os ponteiros sao absolutos
    TListaNosInternos *tabIndice = cria_nos_internos(D, 3,
                                                     cria_no_interno(D, 1, -1, 0, 1 * 2 + 1,
                                                                     1 * tamanho_no_interno(D),
                                                                     2 * tamanho_no_interno(D),
                                                                     37
                                                     ),
                                                     cria_no_interno(D, 2, 0, 1, 2 * 2 + 1,
                                                                     0 * tamanho_no_folha(D),
                                                                     1 * tamanho_no_folha(D),
                                                                     2 * tamanho_no_folha(D),
                                                                     15,
                                                                     25
                                                     ),
                                                     cria_no_interno(D, 2, 0, 1, 2 * 2 + 1,
                                                                     3 * tamanho_no_folha(D),
                                                                     4 * tamanho_no_folha(D),
                                                                     5 * tamanho_no_folha(D),
                                                                     39,
                                                                     55
                                                     )
    );
    salva_nos_internos(D, NOME_ARQUIVO_INDICE, tabIndice);

    // Estrutura do No Folha: pont_pai, pont_prox, m, registros de pizzas
    TListaNosFolhas *tabDados = cria_nos_folhas(D, 6,
                                                cria_no_folha(D, 1 * tamanho_no_interno(D), 1 * tamanho_no_folha(D), 2,
                                                              pizza(10, "Calabresa", "Salgada", 30),
                                                              pizza(13, "Portuguesa", "Salgada", 32)
                                                ),
                                                cria_no_folha(D, 1 * tamanho_no_interno(D), 2 * tamanho_no_folha(D), 2,
                                                              pizza(15, "Bianca", "Salgada", 30),
                                                              pizza(20, "Lombinho", "Salgada", 32)
                                                ),
                                                cria_no_folha(D, 1 * tamanho_no_interno(D), 3 * tamanho_no_folha(D), 2,
                                                              pizza(25, "Banana", "Doce", 25),
                                                              pizza(35, "Camarao", "Salgada", 35)
                                                ),
                                                cria_no_folha(D, 2 * tamanho_no_interno(D), 4 * tamanho_no_folha(D), 2,
                                                              pizza(37, "Chocolate com Morango", "Doce", 28),
                                                              pizza(38, "Calabresa Acebolada", "Salgada", 30)
                                                ),
                                                cria_no_folha(D, 2 * tamanho_no_interno(D), 5 * tamanho_no_folha(D), 2,
                                                              pizza(39, "Frango com Catupiry", "Salgada", 35),
                                                              pizza(50, "Marguerita", "Salgada", 28)
                                                ),
                                                cria_no_folha(D, 2 * tamanho_no_interno(D), -1, 3,
                                                              pizza(55, "Morango com Nutella", "Doce Especial", 45),
                                                              pizza(60, "Presunto, Queijo e Tomate", "Salgada", 31),
                                                              pizza(70, "Muzzarela", "Salgada", 28)
                                                )
    );
    salva_nos_folhas(D, NOME_ARQUIVO_DADOS, tabDados);
    free(tabMetadados);
    libera_nos_internos(tabIndice);
    libera_nos_folhas(D, tabDados);
}

BEGIN_TESTS();

    TEST("Busca 1 -- chave procurada esta na raiz. Raiz eh folha");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h1();

        pont = busca(13, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);
    }

    TEST("Busca 2 -- chave procurada nao esta na arvore. Raiz eh folha");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h1();

        pont = busca(6, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);
    }

    TEST("Busca 3 -- chave esta na arvore");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h2();

        pont = busca(20, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(1 * tamanho_no_folha(D), pont);
    }

    TEST("Busca 4 -- chave nao esta na arvore");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h2();

        pont = busca(16, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(1 * tamanho_no_folha(D), pont);
    }

    TEST("Busca 5 -- chave esta na arvore");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h3();

        pont = busca(37, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(3 * tamanho_no_folha(D), pont);
    }

    TEST("Busca 6 -- chave nao esta na arvore");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h3();

        pont = busca(26, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(2 * tamanho_no_folha(D), pont);
    }


    TEST("Insercao 1 em arvore de altura H = 1, sem particionamento");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h1();

        pont = insere(11, "Cinco Queijos", "Especial", 40, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE,
                      NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);

        TMetadados *tabMetadados = metadados(D, 0, 1, 0, 1 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 1,
                                                    cria_no_folha(D, -1, -1, 3,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(11, "Cinco Queijos", "Especial", 40),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)));
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Insercao 2 em arvore de altura H = 2, sem particionamento");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h2();

        pont = insere(11, "Cinco Queijos", "Especial", 40, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE,
                      NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 2, -1, 1, 2 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         15,
                                                                         25
                                                         )
        );
        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 3,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(11, "Cinco Queijos", "Especial", 40),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 2,
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(20, "Lombinho", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 3,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    )
        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Insercao 3 em arvore de altura H = 2. Exige particionamento de uma pagina folha");
    if (!skip) {
        int pont = INT_MAX;
        // Arvore tem um dos nohs folha cheio. E eh neste noh que a insercao ocorrera
        monta_arvore_h2_cheia();

        pont = insere(16, "Presunto e Queijo", "Salgada", 30, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE,
                      NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(1 * tamanho_no_folha(D), pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 4 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 3, -1, 1, 3 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         3 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         15,
                                                                         20,
                                                                         25
                                                         )
        );
        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 4,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 3 * tamanho_no_folha(D), 2,
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(16, "Presunto e Queijo", "Salgada", 30)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 3,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 3,
                                                                  pizza(20, "Lombinho", "Salgada", 32),
                                                                  pizza(21, "Prestigio", "Doce", 25),
                                                                  pizza(23, "Presunto Parma com Rucula", "Especial", 50)
                                                    )
        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Insercao 4 em arvore de altura H = 2, chave de registro ja existe -- nao inserir");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h2();

        pont = insere(13, "Portuguesa Especial", "Salgada", 35, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE,
                      NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(-1, pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 2, -1, 1, 2 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         15,
                                                                         25
                                                         )
        );
        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 2,
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(20, "Lombinho", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 3,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    )
        );

        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Insercao 5 em arvore de altura H = 1 cheia, que causa aumento na altura da arvore");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h1_cheia();

        pont = insere(11, "Cinco Queijos", "Especial", 40, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE,
                      NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, 1 * tamanho_no_interno(D), 2 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 1, -1, 1, 1 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         13
                                                         )
        );
        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 2,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(11, "Cinco Queijos", "Especial", 40)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 3,
                                                                  pizza(13, "Portuguesa", "Salgada", 32),
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(26, "Frango", "Salgada", 31)
                                                    )
        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Insercao 6 em arvore de altura H = 2 completa. Folha esta cheia, raiz tambem");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h2_completa();

        pont = insere(22, "Banana com Chocolate", "Doce", 30, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE,
                      NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(5 * tamanho_no_folha(D), pont);

        TMetadados *tabMetadados = metadados(D, 2 * tamanho_no_interno(D), 0, 3 * tamanho_no_interno(D),
                                             6 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 3,
                // Antigo noh raiz
                                                         cria_no_interno(D, 2, 2 * tamanho_no_interno(D), 1, 2 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         5 * tamanho_no_folha(D),
                                                                         15,
                                                                         21
                                                         ),
                // Noh que surgiu do particionamento da raiz
                                                         cria_no_interno(D, 2, 2 * tamanho_no_interno(D), 1, 2 * 2 + 1,
                                                                         2 * tamanho_no_folha(D),
                                                                         3 * tamanho_no_folha(D),
                                                                         4 * tamanho_no_folha(D),
                                                                         40,
                                                                         70
                                                         ),
                // Novo noh raiz
                                                         cria_no_interno(D, 1, -1, 0, 1 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_interno(D),
                                                                         25
                                                         )
        );
        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 6,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 5 * tamanho_no_folha(D), 2,
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(20, "Lombinho", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 3 * tamanho_no_folha(D),
                                                                  3,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 4 * tamanho_no_folha(D),
                                                                  2,
                                                                  pizza(40, "Camarao com Cogumelos", "Especial", 51),
                                                                  pizza(45, "Quatro Queijos", "Salgada", 35)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), -1, 2,
                                                                  pizza(70, "Muzzarela", "Salgada", 28),
                                                                  pizza(77, "Atum", "Salgada", 29)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 3,
                                                                  pizza(21, "Prestigio", "Doce", 25),
                                                                  pizza(22, "Banana com Chocolate", "Doce", 30),
                                                                  pizza(23, "Presunto Parma com Rucula", "Especial", 50)
                                                    )
        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);
        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Exclusao 1 em arvore de altura H = 2 cheia, nao eh necessario concatenacao");
    if (!skip) {
        int pont = INT_MAX;
        // Arvore tem um dos nohs folha cheio. E eh neste noh que a excusao ocorrera
        monta_arvore_h2_cheia();

        pont = exclui(20, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(1 * tamanho_no_folha(D), pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 2, -1, 1, 2 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         15,
                                                                         25
                                                         )
        );
        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 3,
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(21, "Prestigio", "Doce", 25),
                                                                  pizza(23, "Presunto Parma com Rucula", "Especial", 50)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 3,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    )
        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);
        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Exclusao 2 em arvore de altura H = 2, eh necessario redistribuição");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h2();

        pont = exclui(20, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(1 * tamanho_no_folha(D), pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 2, -1, 1, 2 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         15,
                                                                         35
                                                         )
        );

        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 2,
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(25, "Banana", "Doce", 25)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 2,
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    )
        );

        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Exclusao 3 em arvore de altura H = 2, eh necessario redistribuicao");
    if (!skip) {
        int pont = INT_MAX;
        // Arvore tem um dos nohs folha cheio. E eh neste noh que a excusao ocorrera
        monta_arvore_h2_cheia();

        pont = exclui(13, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 2, -1, 1, 2 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         20,
                                                                         25
                                                         )
        );
        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(15, "Bianca", "Salgada", 30)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 3,
                                                                  pizza(20, "Lombinho", "Salgada", 32),
                                                                  pizza(21, "Prestigio", "Doce", 25),
                                                                  pizza(23, "Presunto Parma com Rucula", "Especial", 50)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 3,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    )
        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Exclusao 4 em arvore de altura H = 2, eh necessario concatenação");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h2();

        pont = exclui(10, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);

        TMetadados *tabMetadados = metadados(D, 0, 0, tamanho_no_interno(D), 3 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                                                         cria_no_interno(D, 1, -1, 1, 2 * 1 + 1,
                                                                         0,
                                                                         2 * tamanho_no_folha(D),
                                                                         25
                                                         )
        );

        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 3,
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 3,
                                                                  pizza(13, "Portuguesa", "Salgada", 32),
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(20, "Lombinho", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D),
                                                                  2, //Esse noh eh lixo -- fica como estava -- nao ha mais ninguem apontando para ele
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(20, "Lombinho", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 3,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35),
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28)
                                                    )
        );

        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Exclusao 5 em arvore de altura H = 3, eh necessario concatenação, causa diminuicao da altura da arvore");
    if (!skip) {
        int pont = INT_MAX;
        monta_arvore_h3();

        pont = exclui(13, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, 2);
        ASSERT_EQUAL(0, pont);

        //raiz da arvore mudou para 1 * tamanho_no_interno
        TMetadados *tabMetadados = metadados(D, 1 * tamanho_no_interno(D), 0, 3 * tamanho_no_interno(D), 6 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 3,
                                                         cria_no_interno(D, 1, -1, 0, 1 * 2 +
                                                                                      1, //esse noh eh lixo -- era a antiga raiz -- permanece como estava
                                                                         1 * tamanho_no_interno(D),
                                                                         2 * tamanho_no_interno(D),
                                                                         37
                                                         ),
                                                         cria_no_interno(D, 4, 0, 1, 2 * 2 + 1, //essa eh a nova raiz da arvore
                                                                         0 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         3 * tamanho_no_folha(D),
                                                                         4 * tamanho_no_folha(D),
                                                                         5 * tamanho_no_folha(D),
                                                                         25,
                                                                         37,
                                                                         39,
                                                                         55
                                                         ),
                                                         cria_no_interno(D, 2, 0, 1, 2 * 2 +
                                                                                     1, //esse noh eh lixo -- permanece como estava
                                                                         3 * tamanho_no_folha(D),
                                                                         4 * tamanho_no_folha(D),
                                                                         5 * tamanho_no_folha(D),
                                                                         39,
                                                                         55
                                                         )
        );

        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 6,
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 2 * tamanho_no_folha(D),
                                                                  3,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(20, "Lombinho", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 2 * tamanho_no_folha(D),
                                                                  2, //esse noh eh lixo -- permanece como estava -- nao ha mais ninguem apontando para ele
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(20, "Lombinho", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 3 * tamanho_no_folha(D),
                                                                  2,
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(35, "Camarao", "Salgada", 35)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 4 * tamanho_no_folha(D),
                                                                  2,
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28),
                                                                  pizza(38, "Calabresa Acebolada", "Salgada", 30)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 5 * tamanho_no_folha(D),
                                                                  2,
                                                                  pizza(39, "Frango com Catupiry", "Salgada", 35),
                                                                  pizza(50, "Marguerita", "Salgada", 28)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), -1,
                                                                  3,
                                                                  pizza(55, "Morango com Nutella", "Doce Especial", 45),
                                                                  pizza(60, "Presunto, Queijo e Tomate", "Salgada", 31),
                                                                  pizza(70, "Muzzarela", "Salgada", 28)
                                                    )
        );

        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Carga de Dados Inicial 1, raiz eh folha");
    if (!skip) {
        gera_arquivo_carga_inicial_1();

        carrega_dados(D, NOME_ARQUIVO_INICIAL, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS);

        TMetadados *tabMetadados = metadados(D, 0, 1, 0, 1 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 1,
                                                    cria_no_folha(D, -1, -1, 4,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32),
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(26, "Frango", "Salgada", 31)));
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Carga de Dados Inicial 2, folha e 2 nohs");
    if (!skip) {
        gera_arquivo_carga_inicial_2();

        carrega_dados(D, NOME_ARQUIVO_INICIAL, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS);

        //Estrutura dos metadados: (int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre)
        TMetadados *tabMetadados = metadados(D, 0, 0, 1 * tamanho_no_interno(D), 2 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 1,
                //Estrutura no interno: int d, int m, int pont_pai, int aponta_folha, int size, ...
                                                         cria_no_interno(D, 1, -1, 1, 1 * 2 + 1,
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         15
                                                         )
        );

        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 2,
                //Estrutura no folha: int d, int pont_pai, int pont_prox, int m, ...
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2,
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, -1, 4,
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(25, "Banana", "Doce", 25),
                                                                  pizza(26, "Frango", "Salgada", 31),
                                                                  pizza(35, "Camarao", "Salgada", 35)
                                                    )
        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

    TEST("Carga de Dados Inicial 3, 50 registros, 11 nohs internos, 24 folhas");
    if (!skip) {
        gera_arquivo_carga_inicial_3();

        carrega_dados(D, NOME_ARQUIVO_INICIAL, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS);

        //Estrutura dos metadados: (int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre)
        TMetadados *tabMetadados = metadados(D, 8 * tamanho_no_interno(D), 0, 11 * tamanho_no_interno(D),
                                             24 * tamanho_no_folha(D));
        TMetadados *tabMetadadosSaida = le_arq_metadados(NOME_ARQUIVO_METADADOS);
        ASSERT_EQUAL_CMP(D, tabMetadados, tabMetadadosSaida, cmp_metadados);
        free(tabMetadados);
        free(tabMetadadosSaida);

        TListaNosInternos *tabIndice = cria_nos_internos(D, 11,
                //Estrutura no interno: int d, int m, int pont_pai, int aponta_folha, int size, ...
                                                         cria_no_interno(D, 2, 2 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n3
                                                                         0,
                                                                         1 * tamanho_no_folha(D),
                                                                         2 * tamanho_no_folha(D),
                                                                         15,
                                                                         20
                                                         ),
                                                         cria_no_interno(D, 2, 2 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n8
                                                                         3 * tamanho_no_folha(D),
                                                                         4 * tamanho_no_folha(D),
                                                                         5 * tamanho_no_folha(D),
                                                                         26,
                                                                         31
                                                         ),
                                                         cria_no_interno(D, 2, 8 * tamanho_no_interno(D), 0,
                                                                         2 * 2 + 1, //n9
                                                                         0,
                                                                         1 * tamanho_no_interno(D),
                                                                         3 * tamanho_no_interno(D),
                                                                         23,
                                                                         37
                                                         ),
                                                         cria_no_interno(D, 2, 2 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n13
                                                                         6 * tamanho_no_folha(D),
                                                                         7 * tamanho_no_folha(D),
                                                                         8 * tamanho_no_folha(D),
                                                                         39,
                                                                         42
                                                         ),
                                                         cria_no_interno(D, 2, 7 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n19
                                                                         9 * tamanho_no_folha(D),
                                                                         10 * tamanho_no_folha(D),
                                                                         11 * tamanho_no_folha(D),
                                                                         60,
                                                                         77
                                                         ),
                                                         cria_no_interno(D, 2, 7 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n21
                                                                         12 * tamanho_no_folha(D),
                                                                         13 * tamanho_no_folha(D),
                                                                         14 * tamanho_no_folha(D),
                                                                         87,
                                                                         93
                                                         ),
                                                         cria_no_interno(D, 2, 7 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n24
                                                                         15 * tamanho_no_folha(D),
                                                                         16 * tamanho_no_folha(D),
                                                                         17 * tamanho_no_folha(D),
                                                                         102,
                                                                         110
                                                         ),
                                                         cria_no_interno(D, 4, 8 * tamanho_no_interno(D), 0,
                                                                         4 * 2 + 1, //n25
                                                                         4 * tamanho_no_interno(D),
                                                                         5 * tamanho_no_interno(D),
                                                                         6 * tamanho_no_interno(D),
                                                                         9 * tamanho_no_interno(D),
                                                                         10 * tamanho_no_interno(D),
                                                                         83,
                                                                         96,
                                                                         112,
                                                                         133
                                                         ),
                                                         cria_no_interno(D, 1, -1, 0, 1 * 2 + 1, //n26 RAIZ
                                                                         2 * tamanho_no_interno(D),
                                                                         7 * tamanho_no_interno(D),
                                                                         50
                                                         ),
                                                         cria_no_interno(D, 2, 7 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n30
                                                                         18 * tamanho_no_folha(D),
                                                                         19 * tamanho_no_folha(D),
                                                                         20 * tamanho_no_folha(D),
                                                                         119,
                                                                         125
                                                         ),
                                                         cria_no_interno(D, 2, 7 * tamanho_no_interno(D), 1,
                                                                         2 * 2 + 1, //n34
                                                                         21 * tamanho_no_folha(D),
                                                                         22 * tamanho_no_folha(D),
                                                                         23 * tamanho_no_folha(D),
                                                                         137,
                                                                         141
                                                         )

        );

        TListaNosInternos *tabIndiceSaida = le_nos_internos(D, NOME_ARQUIVO_INDICE);
        ASSERT_EQUAL_CMP(D, tabIndice, tabIndiceSaida, cmp_nos_internos);
        libera_nos_internos(tabIndice);
        libera_nos_internos(tabIndiceSaida);

        TListaNosFolhas *tabDados = cria_nos_folhas(D, 24,
                //Estrutura no folha: int d, int pont_pai, int pont_prox, int m, ...
                                                    cria_no_folha(D, 0, 1 * tamanho_no_folha(D), 2, //n1
                                                                  pizza(10, "Calabresa", "Salgada", 30),
                                                                  pizza(13, "Portuguesa", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 0, 2 * tamanho_no_folha(D), 2, //n2
                                                                  pizza(15, "Bianca", "Salgada", 30),
                                                                  pizza(18, "Estrogonoff com Cogumelos", "Especial", 40)
                                                    ),
                                                    cria_no_folha(D, 0, 3 * tamanho_no_folha(D), 2, //n4
                                                                  pizza(20, "Lombinho", "Salgada", 32),
                                                                  pizza(21, "Prestigio", "Doce", 25)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 4 * tamanho_no_folha(D),
                                                                  2, //n5
                                                                  pizza(23, "Presunto Parma com Rucula", "Especial",
                                                                        50),
                                                                  pizza(25, "Banana", "Doce", 25)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 5 * tamanho_no_folha(D),
                                                                  2, //n6
                                                                  pizza(26, "Frango", "Salgada", 31),
                                                                  pizza(28, "Havaiana", "Salgada", 33)
                                                    ),
                                                    cria_no_folha(D, 1 * tamanho_no_interno(D), 6 * tamanho_no_folha(D),
                                                                  2, //n7
                                                                  pizza(31, "Banana com Nutela", "Doce Especial", 42),
                                                                  pizza(35, "Camarao", "Salgada", 35)
                                                    ),
                                                    cria_no_folha(D, 3 * tamanho_no_interno(D), 7 * tamanho_no_folha(D),
                                                                  2, //n10
                                                                  pizza(37, "Chocolate com Morango", "Doce", 28),
                                                                  pizza(38, "Calabresa Acebolada", "Salgada", 30)
                                                    ),
                                                    cria_no_folha(D, 3 * tamanho_no_interno(D), 8 * tamanho_no_folha(D),
                                                                  2, //n11
                                                                  pizza(39, "Frango com Catupiry", "Salgada", 35),
                                                                  pizza(40, "Camarao com Cogumelos", "Especial", 51)
                                                    ),
                                                    cria_no_folha(D, 3 * tamanho_no_interno(D), 9 * tamanho_no_folha(D),
                                                                  2, //n12
                                                                  pizza(42, "Queijo Brie com Damasco", "Doce Especial",
                                                                        51),
                                                                  pizza(45, "Quatro Queijos", "Salgada", 35)
                                                    ),
                                                    cria_no_folha(D, 4 * tamanho_no_interno(D),
                                                                  10 * tamanho_no_folha(D), 2, //n14
                                                                  pizza(50, "Marguerita", "Salgada", 28),
                                                                  pizza(55, "Morango com Nutella", "Doce Especial", 45)
                                                    ),
                                                    cria_no_folha(D, 4 * tamanho_no_interno(D),
                                                                  11 * tamanho_no_folha(D), 2, //n15
                                                                  pizza(60, "Presunto, Queijo e Tomate", "Salgada", 31),
                                                                  pizza(70, "Muzzarela", "Salgada", 28)
                                                    ),
                                                    cria_no_folha(D, 4 * tamanho_no_interno(D),
                                                                  12 * tamanho_no_folha(D), 2, //n16
                                                                  pizza(77, "Atum", "Salgada", 29),
                                                                  pizza(79, "Aliche", "Salgada", 30)
                                                    ),
                                                    cria_no_folha(D, 5 * tamanho_no_interno(D),
                                                                  13 * tamanho_no_folha(D), 2, //n17
                                                                  pizza(83, "Picadinho de Filet", "Salgada", 35),
                                                                  pizza(85, "Calabresa Picante", "Salgada", 31)
                                                    ),
                                                    cria_no_folha(D, 5 * tamanho_no_interno(D),
                                                                  14 * tamanho_no_folha(D), 2, //n18
                                                                  pizza(87, "Lombinho com Catupiry", "Salgada", 37),
                                                                  pizza(90, "Brocolis com Alho", "Vegetariana", 28)
                                                    ),
                                                    cria_no_folha(D, 5 * tamanho_no_interno(D),
                                                                  15 * tamanho_no_folha(D), 2, //n20
                                                                  pizza(93, "Rucula", "Vegetariana", 29),
                                                                  pizza(95, "Rucula com Tomate Seco", "Especial", 40)
                                                    ),
                                                    cria_no_folha(D, 6 * tamanho_no_interno(D),
                                                                  16 * tamanho_no_folha(D), 2, //n21
                                                                  pizza(96, "Palmito", "Vegetariana", 37),
                                                                  pizza(98, "Caprese", "Vegetariana", 39)
                                                    ),
                                                    cria_no_folha(D, 6 * tamanho_no_interno(D),
                                                                  17 * tamanho_no_folha(D), 2, //n22
                                                                  pizza(102, "Presunto Parma com Muzzarela de Bufala",
                                                                        "Especial", 57),
                                                                  pizza(103, "Presunto e Azeitonas", "Salgada", 35)
                                                    ),
                                                    cria_no_folha(D, 6 * tamanho_no_interno(D),
                                                                  18 * tamanho_no_folha(D), 2, //n23
                                                                  pizza(110, "Calabresa com Catupiry", "Salgada", 32),
                                                                  pizza(111,
                                                                        "Linguica Artesanal com Cubos de Queijo Qualho",
                                                                        "Especial", 50)
                                                    ),
                                                    cria_no_folha(D, 9 * tamanho_no_interno(D),
                                                                  19 * tamanho_no_folha(D), 2, //n27
                                                                  pizza(112, "Abobrinha", "Vegetariana", 29),
                                                                  pizza(115, "Alho e Oleo", "Vegetariana", 25)
                                                    ),
                                                    cria_no_folha(D, 9 * tamanho_no_interno(D),
                                                                  20 * tamanho_no_folha(D), 2, //n28
                                                                  pizza(119, "Peperoni", "Salgada", 31),
                                                                  pizza(121, "Abobrinha, Beringela e Tomate Seco",
                                                                        "Vegetariana", 40)
                                                    ),
                                                    cria_no_folha(D, 9 * tamanho_no_interno(D),
                                                                  21 * tamanho_no_folha(D), 2, //n29
                                                                  pizza(125, "Cogumelhos Especiais", "Especial", 57),
                                                                  pizza(132, "Peito de Peru", "Salgada", 24)
                                                    ),
                                                    cria_no_folha(D, 10 * tamanho_no_interno(D),
                                                                  22 * tamanho_no_folha(D), 2, //n31
                                                                  pizza(133, "Coracao de Galinha", "Salgada", 30),
                                                                  pizza(135, "Milho com Bacon", "Salgada", 32)
                                                    ),
                                                    cria_no_folha(D, 10 * tamanho_no_interno(D),
                                                                  23 * tamanho_no_folha(D), 2, //n32
                                                                  pizza(137, "Frango, Milho e Palmito", "Salgada", 27),
                                                                  pizza(140, "Camarao com Catupiry", "Salgada", 40)
                                                    ),
                                                    cria_no_folha(D, 10 * tamanho_no_interno(D), -1, 4, //n33
                                                                  pizza(141, "Queijo Brie com Figo", "Doce Especial",
                                                                        55),
                                                                  pizza(142, "Presunto e Azeitonas com Catupiry",
                                                                        "Salgada", 30),
                                                                  pizza(144, "Peperoni com Catupiry", "Salgada", 29),
                                                                  pizza(146, "Brocolis com Bacon", "Salgada", 29)
                                                    )

        );
        TListaNosFolhas *tabDadosSaida = le_nos_folhas(D, NOME_ARQUIVO_DADOS);

        ASSERT_EQUAL_CMP(D, tabDados, tabDadosSaida, cmp_nos_folhas);
        libera_nos_folhas(D, tabDados);
        libera_nos_folhas(D, tabDadosSaida);
    }

END_TESTS();