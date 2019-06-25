#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <stdlib.h>

#include "arvore_b_mais.h"
#include "lista_pizzas.h"
#include "metadados.h"
#include "no_interno.h"
#include "no_folha.h"


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

//retorna o elemento e a posição dele
int busca_binaria(void *r,int *pos, int chave, int esq, int dir, int folha){

    if(!r)return INT_MIN;
    int meio = (esq + dir)/2;
    *pos = meio;
    if(folha){
        if(dir >= esq){
            TNoFolha *raiz = r;
            if(raiz->pizzas[meio]->cod < chave){
                return busca_binaria(raiz, pos, chave,meio+1, dir, folha);
            }
            else if(raiz->pizzas[meio]->cod == chave){
                *pos = meio;
                return chave;
            }
            return busca_binaria(raiz, pos, chave, esq, meio - 1, folha);
        }
    }else{
        if(dir >= esq){
            TNoInterno *raiz = r;
            if(raiz->chaves[meio] < chave){
                return busca_binaria(raiz, pos, chave,meio+1, dir, folha);
            }
            else if(raiz->chaves[meio] == chave && raiz->aponta_folha){
                *pos = raiz->p[meio + 1];
                return chave;
            }
            return busca_binaria(raiz, pos, chave, esq, meio - 1, folha);
        }
    }
    return -1;
}

int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//TODO: Inserir aqui o codigo do algoritmo
    TMetadados * metadados = le_arq_metadados(nome_arquivo_metadados);
    TNoFolha *raizF = NULL;
    TNoInterno *raizI = NULL;
    FILE *indices = fopen(nome_arquivo_indice, "rb");

    if(!indices){
        perror("erro");
        return -1;
    }

    FILE *dados = fopen(nome_arquivo_dados, "rb");

    if(!dados){
        perror("erro");
        return -1;
    }

    if(!metadados)return -1;

    else{
        if(metadados->raiz_folha)raizF = le_no_folha(d, dados);
        else raizI = le_no_interno(d, indices);
    }

    if(!raizI && !raizF)return -1;

    int value = 0;
    int pos;
    int p_no = 0;

    while(raizI){//percorre o arquivo de indices
        pos = 0;
        value = busca_binaria(raizI, &pos, cod, 0, raizI->m-1, 0);
        //imprime_no_interno(2, raizI);
        int i = pos;
        if(value != cod && raizI->p[0] != -1){//se o elemento não está no nó e ele é interno
            if(raizI->chaves[i] > cod){
                p_no = raizI->p[pos];
                if(raizI->aponta_folha)break;
                fseek(indices, p_no, SEEK_SET);
                raizI = le_no_interno(d, indices);
            }else{
                p_no = raizI->p[pos+1];
                if(raizI->aponta_folha)break;
                fseek(indices, p_no, SEEK_SET);
                raizI = le_no_interno(d, indices);
            }
        }else{

        }
    }
    return p_no;
}
//int busca_binaria(void *r,int *pos, int chave, int esq, int dir, int folha)
//retorna o elemento e a posição dele

int buscaFolha(int pos, char *nome_arquivo_dados, int cod, int d){

    FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
    fseek(arq_Dados, pos, SEEK_SET);
    TNoFolha *noFolha = le_no_folha(d, arq_Dados);
    int x = 0;
    if(noFolha){
        int p = busca_binaria(noFolha, &x, cod, 0, noFolha->m-1, 1);
        if(p == -1) return 0;
        else return 1;
    }
    return 0;
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

TNoFolha *retira_pizza(TNoFolha *no, int pos){
    int i;
    for(i = pos; i < no->m - 1; i++){
        if(no->pizzas[i+1]){
            no->pizzas[i] = no->pizzas[i+1];
        }
    }
    no->pizzas[no->m-1] = NULL;
    no->m--;
    return no;
}

void redistribui(TNoInterno *p, TNoInterno *q, TNoInterno *w, int cod, int pos, int dir){
    p = retira_pizza(p, pos);
    if(dir > 0){

    }else{

    }
}

int irmao(FILE *dados, TNoFolha *a, TNoFolha *b){

}

//int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//TODO: Inserir aqui o codigo do algoritmo de remocao

	int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
    FILE * dados = fopen(nome_arquivo_dados, "rb+");
    if(!dados)return -1;
    FILE *indice = fopen(nome_arquivo_indice, "rb+");
    fseek(dados, pos, SEEK_SET);
    TNoFolha * no = le_no_folha(d, dados);
    //imprime_no_folha(d, no);
    int p = 0;
    busca_binaria(no, &p, cod, 0, no->m, 1);


    if(no->m > d || no->pont_pai == -1){//nao tem concatenacao nem redistribuicao

        no = retira_pizza(no, p);
        //imprime_no_folha(d, no);
        fseek(dados, pos, SEEK_SET);
        salva_no_folha(d, no, dados);
        fclose(dados);

    }else{//caso de haver redistribuição ou concatenação

        fseek(dados, no->pont_prox, SEEK_SET);
        TNoFolha *p = le_no_folha(2, dados);
        TNoInterno * w = NULL;
        if(p->pont_pai == no->pont_pai){
            fseek(indice, no->pont_pai, SEEK_SET);
            w = le_no_interno(2, indice);
        }
        TNoFolha *q = no;

        if(p->m + q->m >= 2*d){//redistribuição
            //redistribui(p, q, w, cod);
        }else{//concatenacao
            //ahhhhhhh vai tomar no cuuuuuu
        }
    }
    return pos;
}

void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
    //TODO: Implementar essa funcao
}
