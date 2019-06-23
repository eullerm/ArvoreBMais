#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <stdlib.h>
#include <mem.h>

#include "arvore_b_mais.h"
#include "lista_pizzas.h"
#include "metadados.h"
#include "no_folha.h"
#include "lista_nos_internos.h"

void checaMetadados(char *nome_arquivo_metadados, int d){

    TMetadados *metadados1 = le_arq_metadados(nome_arquivo_metadados);
    if(!metadados1){//Coloca valores no metadados
        metadados1 = metadados(d, 0, 1, 0, 0);
        salva_arq_metadados(nome_arquivo_metadados, metadados1);
        free(metadados1);
    }

}

void insereNoInterno(int cod, int d, char *nome_arquivo_indice, FILE *arq_Dados ,TMetadados *metadados, int pos){

    FILE *arq_Indices = fopen(nome_arquivo_indice, "rb+");
    fseek(arq_Indices, pos, SEEK_SET);

    TNoInterno *noPai = le_no_interno(d, arq_Indices);

    if(noPai->m < 2*d) {//Caso caiba

        TNoInterno *novo = no_interno_vazio(d);
        novo->pont_pai = noPai->pont_pai;
        novo->aponta_folha = noPai->aponta_folha;

        //Guarda todos de forma ordenada no nó
        int j = 0;
        novo->p[j] = noPai->p[j];
        while (j < noPai->m && cod > noPai->chaves[j]) {
            novo->chaves[j] = noPai->chaves[j];
            j++;
            novo->p[j] = noPai->p[j];
        }

        novo->chaves[j] = cod;
        novo->m = noPai->m + 1;

        if(novo->aponta_folha) novo->p[j + 1] = metadados->pont_prox_no_folha_livre;
        else novo->p[j + 1] = metadados->pont_prox_no_interno_livre;

        while (j + 1 < noPai->m + 1) {
            novo->chaves[j + 1] = noPai->chaves[j];
            j++;
            novo->p[j + 1] = noPai->p[j];
        }
        //------------------------------------------------------//
        fseek(arq_Indices, pos, SEEK_SET);
        salva_no_interno(d, novo, arq_Indices);
        libera_no_interno(novo);

    }else{//Caso precise dividir

        int guardaChaves[(2 * d) + 1];
        int guardaP[(2 * d) + 2];
        int j = 0;
        //Guarda todas em um vetor
        guardaP[j] = noPai->p[j];
        while (j < noPai->m && cod > noPai->chaves[j]) {
            guardaChaves[j] = noPai->chaves[j];
            j++;
            guardaP[j] = noPai->p[j];
        }

        guardaChaves[j] = cod;
        if(noPai->aponta_folha) guardaP[j + 1] = metadados->pont_prox_no_folha_livre;
        else guardaP[j + 1] = metadados->pont_prox_no_interno_livre;

        while (j + 1 < noPai->m + 1) {
            guardaChaves[j + 1] = noPai->chaves[j];
            j++;
            guardaP[j + 1] = noPai->p[j];
        }
        //-------------------------------------------------------------------//

        //Acha o meio na hora de mandar pra cima.
        int meio;
        if (j % 2 == 0) meio = j / 2;
        else meio = j / 2 + 1;

        TNoInterno *novo = no_interno_vazio(d);
        novo->pont_pai = noPai->pont_pai;
        novo->aponta_folha = noPai->aponta_folha;
        TNoInterno *novo2 = no_interno_vazio(d);
        novo2->pont_pai = noPai->pont_pai;
        novo2->aponta_folha = noPai->aponta_folha;

        //Divide o vetor em 2 novos no
        int k1 = 0;
        int k2 = 0;
        novo->p[k1] = noPai->p[k1];
        while (k1 < meio) {
            novo->chaves[k1] = guardaChaves[k1];
            k1++;
            novo->p[k1] = guardaP[k1];
            novo->m++;
        }

        novo2->p[k2] = guardaP[meio + 1];
        TNoFolha *mudaP;
        while (k2 < meio) {

            if(novo2->aponta_folha) { //Para alterar o pai do no
                fseek(arq_Dados, novo2->p[k2], SEEK_SET);
                mudaP = le_no_folha(d, arq_Dados);
                mudaP->pont_pai = metadados->pont_prox_no_interno_livre;
                fseek(arq_Dados, novo2->p[k2], SEEK_SET);
                salva_no_folha(d, mudaP, arq_Dados);
                libera_no_folha(d, mudaP);
            }

            novo2->chaves[k2] = guardaChaves[k2 + meio + 1];
            k2++;
            novo2->p[k2] = guardaP[k2 + meio + 1];
            novo2->m++;
        }
        if(novo2->aponta_folha) { //Para alterar o pai
            fseek(arq_Dados, novo2->p[k2], SEEK_SET);
            mudaP = le_no_folha(d, arq_Dados);
            mudaP->pont_pai = metadados->pont_prox_no_interno_livre;
            fseek(arq_Dados, novo2->p[k2], SEEK_SET);
            salva_no_folha(d, mudaP, arq_Dados);
            libera_no_folha(d, mudaP);
        }
        //-------------------------------------------------------------------//

        //Se tem pai
        if(noPai->pont_pai!=-1){

            fseek(arq_Indices, pos, SEEK_SET);
            salva_no_interno(d, novo, arq_Indices);
            libera_no_interno(novo);

            fseek(arq_Indices, metadados->pont_prox_no_interno_livre, SEEK_SET);
            salva_no_interno(d, novo2, arq_Indices);
            libera_no_interno(novo2);

            fseek(arq_Indices, 0, SEEK_END);
            metadados->pont_prox_no_interno_livre = ftell(arq_Indices); //Passa a apontar para o final do arquivo.

            insereNoInterno(guardaChaves[meio], d, nome_arquivo_indice, arq_Dados, metadados, noPai->pont_pai);
        }

        else {
            metadados->pont_raiz = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
            novo->pont_pai = metadados->pont_raiz;
            novo2->pont_pai = metadados->pont_raiz;
            TNoInterno *novoPai = no_interno_vazio(d);
            novoPai->p[0] = pos;
            novoPai->p[1] = metadados->pont_prox_no_interno_livre;
            novoPai->chaves[0] = guardaChaves[meio];
            novoPai->m = 1;
            novoPai->aponta_folha = 0;

            fseek(arq_Indices, pos, SEEK_SET);
            salva_no_interno(d, novo, arq_Indices);
            libera_no_interno(novo);

            fseek(arq_Indices, metadados->pont_prox_no_interno_livre, SEEK_SET);
            salva_no_interno(d, novo2, arq_Indices);
            libera_no_interno(novo2);

            salva_no_interno(d, novoPai, arq_Indices);
            libera_no_interno(novoPai);
            fseek(arq_Indices, 0, SEEK_END);
            metadados->pont_prox_no_interno_livre = ftell(arq_Indices);//Passa a apontar para o final do arquivo.

        }

    }

    fclose(arq_Indices);

}

void insereNoFolha(TNoFolha *no, int cod, TPizza *p, int d, FILE *arq_Dados, TMetadados *metadados, int pos, char *nome_arquivo_indice){

    if(no->m < 2*d) {//Caso caiba

        TNoFolha *novo = no_folha_vazio(d);
        novo->pont_pai = no->pont_pai;
        novo->pont_prox = no->pont_prox;

        //Guarda todos clientes de forma ordenada no nó
        int j = 0;
        while (j < no->m && cod > no->pizzas[j]->cod) {
            novo->pizzas[j] = no->pizzas[j];
            j++;
        }

        novo->pizzas[j] = p;
        novo->m = no->m + 1;

        while (j + 1 < no->m + 1) {
            novo->pizzas[j + 1] = no->pizzas[j];
            j++;
        }
        //------------------------------------------------------//
        fseek(arq_Dados, pos, SEEK_SET);

        salva_no_folha(d, novo, arq_Dados);
        libera_no_folha(d, novo);

    } else { //Caso o no esteja cheio

        TPizza *guardaPizza[(2 * d) + 1];
        int j = 0;
        //Guarda todos os clientes em um vetor
        while (j < no->m && cod > no->pizzas[j]->cod) {
            guardaPizza[j] = no->pizzas[j];
            j++;
        }

        guardaPizza[j] = p;

        while (j + 1 < no->m + 1) {
            guardaPizza[j + 1] = no->pizzas[j];
            j++;
        }
        //-------------------------------------------------------------------//

        int meio;
        if (j % 2 == 0) meio = j / 2;
        else meio = j / 2 + 1;

        TNoFolha *novo = no_folha_vazio(d);
        novo->pont_pai = no->pont_pai;
        novo->pont_prox = metadados->pont_prox_no_folha_livre;//Aponta para o local onde o novo2 estara

        TNoFolha *novo2 = no_folha_vazio(d);
        novo2->pont_pai = no->pont_pai;
        novo2->pont_prox = no->pont_prox;

        //Separa nos no
        int k1;
        int k2 = 0;
        for (k1 = 0; k1 < no->m + 1; k1++) {
            if (k1 < meio) {
                novo->pizzas[k1] = guardaPizza[k1];
                novo->m++;
            } else if (k1 >= meio) {
                novo2->pizzas[k2] = guardaPizza[k1];
                novo2->m++;
                k2++;
            }
        }
        //---------------------------------------------------//
        if (no->pont_pai != -1) { //Caso tenha um pai

            fseek(arq_Dados, pos, SEEK_SET);
            salva_no_folha(d, novo, arq_Dados);
            libera_no_folha(d, novo);

            fseek(arq_Dados, metadados->pont_prox_no_folha_livre, SEEK_SET);
            salva_no_folha(d, novo2, arq_Dados);
            int sobe = novo2->pizzas[0]->cod;
            libera_no_folha(d, novo2);

            insereNoInterno(sobe, d, nome_arquivo_indice, arq_Dados, metadados, no->pont_pai);

            fseek(arq_Dados, 0, SEEK_END);
            metadados->pont_prox_no_folha_livre = ftell(arq_Dados);

            //Cria um pai
        }else {

            metadados->pont_raiz = metadados->pont_prox_no_interno_livre;
            novo->pont_pai = metadados->pont_raiz;
            novo2->pont_pai = metadados->pont_raiz;
            metadados->raiz_folha = 0;
            TNoInterno *novoPai = no_interno_vazio(d);
            novoPai->p[0] = pos;
            novoPai->p[1] = metadados->pont_prox_no_folha_livre;
            novoPai->chaves[0] = guardaPizza[meio]->cod;
            novoPai->m = 1;
            novoPai->aponta_folha = 1;

            FILE *arq_Indices = fopen(nome_arquivo_indice, "rb+");
            salva_no_interno(d, novoPai, arq_Indices);
            metadados->pont_prox_no_interno_livre = ftell(arq_Indices);//Passa a apontar para o final do arquivo.
            fclose(arq_Indices);
            libera_no_interno(novoPai);

            fseek(arq_Dados, pos, SEEK_SET);
            salva_no_folha(d, novo, arq_Dados);
            libera_no_folha(d, novo);

            fseek(arq_Dados, metadados->pont_prox_no_folha_livre, SEEK_SET);
            salva_no_folha(d, novo2, arq_Dados);
            libera_no_folha(d, novo2);

            fseek(arq_Dados, 0, SEEK_END);
            metadados->pont_prox_no_folha_livre = ftell(arq_Dados);//Passa a apontar para o final do arquivo.

        }

    }
}

int buscaFolha(int pos, char *nome_arquivo_dados, int cod, int d){

    FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
    fseek(arq_Dados, pos, SEEK_SET);
    TNoFolha *noFolha = le_no_folha(d, arq_Dados);

    if(noFolha) {
        int i = 0;
        while(i < noFolha->m && noFolha->pizzas[i]->cod < cod)
            i++;
        if (i == noFolha->m) i--;
        if (noFolha->pizzas[i]->cod == cod) return 1;
        else return 0;
    }

    else return 0;

}

int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
    FILE *arq_Indice = fopen(nome_arquivo_indice, "rb");

    TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);
    if(metadados->raiz_folha){
        return metadados->pont_raiz;

    }else {
        fseek(arq_Indice, metadados->pont_raiz, SEEK_SET);

        TNoInterno *noInterno = le_no_interno(d, arq_Indice);

        int i;
        while (noInterno) {

            for( i = 0; i < noInterno->m && noInterno->chaves[i] < cod; i++);

            if(i == noInterno->m) i--;
            if (cod >= noInterno->chaves[i]) {
                if (noInterno->aponta_folha) {
                    return noInterno->p[i + 1];
                } else {
                    fseek(arq_Indice, noInterno->p[i + 1], SEEK_SET);
                }
            } else if (cod < noInterno->chaves[i]) {
                if (noInterno->aponta_folha) {
                    return noInterno->p[i];
                } else {
                    fseek(arq_Indice, noInterno->p[i], SEEK_SET);
                }

            }

            noInterno = le_no_interno(d, arq_Indice);
        }
    }
}

int insere(int cod, char *nome, char *categoria, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{

    checaMetadados(nome_arquivo_metadados, d);

    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    if(!buscaFolha(pos, nome_arquivo_dados, cod, d)){

        TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb+");

        fseek(arq_Dados, pos, SEEK_SET);

        TPizza *p = pizza(cod, nome, categoria, preco);

        TNoFolha *noFolha = le_no_folha(d, arq_Dados);

        if(noFolha) insereNoFolha(noFolha, cod, p, d, arq_Dados, metadados, pos, nome_arquivo_indice);

        else{
            TNoFolha *novoFolha = cria_no_folha(d, -1, -1, 1, p);
            fseek(arq_Dados, pos, SEEK_SET);
            salva_no_folha(d, novoFolha, arq_Dados);
            libera_no_folha(d, novoFolha);
            metadados->pont_prox_no_folha_livre = tamanho_no_folha(d);
        }

        //imprime_metadados(metadados);

        salva_arq_metadados(nome_arquivo_metadados, metadados);

        /*rewind(arq_Dados);
        noFolha = le_no_folha(d, arq_Dados);
        while(noFolha){
            imprime_no_folha(d, noFolha);
            noFolha = le_no_folha(d, arq_Dados);
        }*/

        fclose(arq_Dados);


        /*FILE *arqIndices = fopen(nome_arquivo_indice, "rb+");
        TNoInterno * no = le_no_interno(d, arqIndices);
        while(no) {
            imprime_no_interno(d, no);
            no = le_no_interno(d, arqIndices);
        }*/

        pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

        return pos;

    }else{
        return -1;
    }

}

int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
    //TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}

void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{

    FILE *arq_Entrada = fopen(nome_arquivo_entrada, "rb");

    if(arq_Entrada){
        TPizza *pizza;
        fclose(fopen(nome_arquivo_dados, "wb"));//Apaga o conteudo do arquivo.
        fclose(fopen(nome_arquivo_indice, "wb"));
        fclose(fopen(nome_arquivo_metadados, "wb"));
        pizza = le_pizza(arq_Entrada);
        while(pizza) {
            insere(pizza->cod, pizza->nome, pizza->categoria, pizza->preco, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
            pizza = le_pizza(arq_Entrada);
        }
    }

    fclose(arq_Entrada);
}

int imprimirArvore(int d, char *nome_arquivo_dados) {

    FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");

    rewind(arq_Dados);
    TNoFolha *noFolha = le_no_folha(d, arq_Dados);
    if(noFolha){
        while (noFolha) {
            imprime_no_folha(d, noFolha);
            noFolha = le_no_folha(d, arq_Dados);
        }
        return 1;
    }else return 0;

}

int buscaPizza(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d){

    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    if(buscaFolha(pos, nome_arquivo_dados, cod, d)) {

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
        fseek(arq_Dados, pos, SEEK_SET);
        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        int i;
        for(i = 0; cod != noFolha->pizzas[i]->cod; i++);
        TPizza *p = noFolha->pizzas[i];
        imprime_pizza(p);
        fclose(arq_Dados);
        return 1;
    }else return 0;
}

void modifica(int cod, char *nome, char *categoria, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d){

    TPizza *p = pizza(cod, nome, categoria, preco);

    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    FILE *arq_Dados = fopen(nome_arquivo_dados, "rb+");
    fseek(arq_Dados, pos, SEEK_SET);
    TNoFolha *noFolha = le_no_folha(d, arq_Dados);
    int i = 0;
    while(noFolha->pizzas[i]->cod != cod) i++;

    noFolha->pizzas[i] = p;

    fseek(arq_Dados, pos, SEEK_SET);
    salva_no_folha(d, noFolha, arq_Dados);

    libera_no_folha(d, noFolha);
    free(p);

    fclose(arq_Dados);
}

int menu(){
    int opt;
    printf("\nOpcoes:\n");
    printf("0. Sair\n");
    printf("1. Inserir\n");
    printf("2. Remover\n");
    printf("3. Imprimir arvore.\n");
    printf("4. Buscar pizza.\n");
    printf("5. Buscar todas pizzas de uma categoria.\n");
    printf("6. Carregar pizzas apartir de um arquivo.\n");
    printf("7. Modificar pizza.\n");
    scanf("%d", &opt);

    return opt;
}

void tarefa(int opt, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d) {

    if (opt == 0){

        printf("Saindo...\n");

    }else if(opt == 1){//Inserir
        int cod;
        char nome[50];
        char categoria[20];
        float preco;
        printf("\nInformacoes da pizza:\n");
        printf("Codigo:\n");
        scanf("%d", &cod);
        printf("Nome\n");
        scanf(" %[^\n]", nome);
        printf("Categoria\n");
        scanf(" %[^\n]", categoria);
        printf("Preço\n");
        scanf("%f", &preco);
        if(insere(cod, nome, categoria, preco, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d) != -1){
            printf("Inserido com Sucesso\n");
        }else printf("Codigo ja exitente\n");

    }else if(opt == 2){//Remover


    }else if(opt == 3){//Imprimir arvore

        if(!imprimirArvore(d, nome_arquivo_dados)) printf("Sem pizzas cadastradas.\n");

    }else if(opt == 4){//Buscar pizza

        int cod;

        printf("Codigo:\n");
        scanf("%d", &cod);
        if(!buscaPizza(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d))
            printf("Pizza nao existente.\n");

    }else if(opt == 5){//Buscar todas as pizzas de uma categoria

        char categoria[20];

        printf("Digite a categoria:\n");
        scanf(" %[^\n]", categoria);

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");

        TNoFolha *noFolha = le_no_folha(d, arq_Dados);

        while(noFolha){

            for(int i = 0; i < noFolha->m; i++){
                if(strcmp(noFolha->pizzas[i]->categoria, categoria) == 0){
                    imprime_pizza(noFolha->pizzas[i]);
                }
            }

            noFolha = le_no_folha(d, arq_Dados);
        }
        fclose(arq_Dados);

    }else if(opt == 6){//Carregar pizzas a partir de um arquivo

        char nome_arquivo_entrada[20];

        printf("Nome do arquivo de entrada(com .txt ou .dat).\n");
        scanf(" %s", nome_arquivo_entrada);
        carrega_dados(d, nome_arquivo_entrada, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados);

    }else if(opt == 7){//modificar pizza.

        int cod;
        char nome[50];
        char categoria[20];
        float preco;

        printf("Codigo:\n");
        scanf("%d", &cod);
        if(buscaPizza(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d)) {
            printf("Novo nome:\n");
            scanf(" %[^\n]", nome);
            printf("Nova categoria:\n");
            scanf(" %[^\n]", categoria);
            printf("Novo preco:\n");
            scanf(" %f", &preco);
            modifica(cod, nome, categoria, preco, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
        }else printf("Não existe essa pizza.\n");

    }else{
        printf("Tarefa invalida.\n");
    }
}

int main(){

    int opt;
    int d;
    char nome_arquivo_metadados[20] = "metadados.dat";
    char nome_arquivo_indice[20] = "indice.dat";
    char nome_arquivo_dados[20] = "pizzas.dat";
    printf("Digite a ordem da arvore.\n");
    scanf(" %d", &d);
    //printf("Nome do arquivo de metadados(com .txt ou .dat).\n");
    //scanf(" %s", nome_arquivo_metadados);
    //printf("Nome do arquivo de indice(com .txt ou .dat).\n");
    //scanf(" %s", nome_arquivo_indice);
    //printf("Nome do arquivo de dados(com .txt ou .dat).\n");
    //scanf(" %s", nome_arquivo_dados);

    int flag;
    printf("\nDigite 1 caso deseje apagar o conteudo existente nesses arquivos e 0 se desejar manter.\n");
    scanf(" %d", &flag);
    if(flag){
        fclose(fopen(nome_arquivo_dados, "wb"));//Apaga o conteudo do arquivo.
        fclose(fopen(nome_arquivo_indice, "wb"));
        fclose(fopen(nome_arquivo_metadados, "wb"));
    }

    do{

        opt = menu();
        tarefa(opt, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    }while(opt);

    return 0;
}