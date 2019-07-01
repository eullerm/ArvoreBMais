#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define DIR 1
#define  ESQ 2
#include <limits.h>
#include <stdlib.h>
#include <mem.h>

#include "arvore_b_mais.h"
#include "lista_pizzas.h"
#include "metadados.h"
#include "no_folha.h"
#include "lista_nos_internos.h"


//Preenche o arquivo metadados se ele estiver vazio.
void checaMetadados(char *nome_arquivo_metadados, int d){

    TMetadados *metadados1 = le_arq_metadados(nome_arquivo_metadados);
    if(!metadados1){//Coloca valores no metadados
        metadados1 = metadados(d, 0, 1, 0, 0);
        salva_arq_metadados(nome_arquivo_metadados, metadados1);
        free(metadados1);
    }

}

//Insere um no interno
void insereNoInterno(int cod, int d, char *nome_arquivo_indice, FILE *arq_Dados ,TMetadados *metadados, int pos, int flag){


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

        fclose(arq_Indices);

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
        TNoInterno *mudaPI;
        while (k2 < meio) {

            if(novo2->aponta_folha) { //Para alterar o pai do no
                fseek(arq_Dados, novo2->p[k2], SEEK_SET);
                mudaP = le_no_folha(d, arq_Dados);
                mudaP->pont_pai = metadados->pont_prox_no_interno_livre;
                fseek(arq_Dados, novo2->p[k2], SEEK_SET);
                salva_no_folha(d, mudaP, arq_Dados);
            }else if(flag == 1){
                fseek(arq_Indices, novo2->p[k2], SEEK_SET);
                mudaPI = le_no_interno(d, arq_Indices);
                mudaPI->pont_pai = metadados->pont_prox_no_interno_livre + flag * tamanho_no_interno(d);
                fseek(arq_Indices, novo2->p[k2], SEEK_SET);
                salva_no_interno(d, mudaPI, arq_Indices);
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
        }else if(flag == 1){
            fseek(arq_Indices, novo2->p[k2], SEEK_SET);
            mudaPI = le_no_interno(d, arq_Indices);
            mudaPI->pont_pai = metadados->pont_prox_no_interno_livre + flag * tamanho_no_interno(d);
            fseek(arq_Indices, novo2->p[k2], SEEK_SET);
            salva_no_interno(d, mudaPI, arq_Indices);
            libera_no_interno(mudaPI);
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

            fclose(arq_Indices);

            insereNoInterno(guardaChaves[meio], d, nome_arquivo_indice, arq_Dados, metadados, noPai->pont_pai, flag+1);

            arq_Indices = fopen(nome_arquivo_indice, "rb+");
            fseek(arq_Indices, 0, SEEK_END);
            metadados->pont_prox_no_interno_livre = ftell(arq_Indices); //Passa a apontar para o final do arquivo.
            fclose(arq_Indices);

        }

        else {
            if(flag!=0){
                metadados->pont_prox_no_interno_livre += flag * tamanho_no_interno(d);
            }

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

            fclose(arq_Indices);
        }

    }
}

//Insere um no folha
void insereNoFolha(TNoFolha *no, int cod, TPizza *p, int d, FILE *arq_Dados, TMetadados *metadados, int pos,
                   char *nome_arquivo_indice){

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

            insereNoInterno(sobe, d, nome_arquivo_indice, arq_Dados, metadados, no->pont_pai, 0);

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
            fseek(arq_Indices, metadados->pont_prox_no_interno_livre, SEEK_SET);
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
                return busca_binaria(raiz, pos, chave,meio + 1, dir, folha);
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
            }else if(raiz->chaves[meio] == chave && raiz->aponta_folha){
                //printf("oi porra desgraça\n");
                *pos = meio;
                return chave;
            }return busca_binaria(raiz, pos, chave, esq, meio - 1, folha);
        }
    }return -1;
}

//Verifica se o codigo já existe
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

int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
    FILE *arq_Indice = fopen(nome_arquivo_indice, "rb");

    TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);
    if(metadados->raiz_folha){
        if(metadados->pont_raiz != -1) return metadados->pont_raiz;
        else{
            return metadados->pont_prox_no_folha_livre;
        }
    }else {
        fseek(arq_Indice, metadados->pont_raiz, SEEK_SET);

        TNoInterno *noInterno = le_no_interno(d, arq_Indice);

        int i;
        while (noInterno) {

            //Busca binaria//
            int inf = 0;
            int sup = noInterno->m - 1;
            int meio = 0;
            while (inf <= sup){
                meio = (inf + sup)/2;
                if (cod < noInterno->chaves[meio])
                    sup = meio-1;
                else
                    inf = meio+1;
            }
            //-------------------//

            if(meio == noInterno->m) meio--;
            if (cod >= noInterno->chaves[meio]) {
                if (noInterno->aponta_folha) {
                    return noInterno->p[meio + 1];
                } else {
                    fseek(arq_Indice, noInterno->p[meio + 1], SEEK_SET);
                    noInterno = le_no_interno(d, arq_Indice);
                }
            } else if (cod < noInterno->chaves[meio]) {
                if (noInterno->aponta_folha) {
                    return noInterno->p[meio];
                } else {
                    fseek(arq_Indice, noInterno->p[meio], SEEK_SET);
                    noInterno = le_no_interno(d, arq_Indice);
                }

            }

        }
    }
}

int insere(int cod, char *nome, char *categoria, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice,
           char *nome_arquivo_dados, int d)
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
            fseek(arq_Dados, 0 ,SEEK_END);
            metadados->pont_prox_no_folha_livre = ftell(arq_Dados);
            metadados->pont_raiz = pos;
            metadados->raiz_folha = 1;
        }

        salva_arq_metadados(nome_arquivo_metadados, metadados);

        fclose(arq_Dados);

        pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

        return pos;

    }else{
        return -1;
    }

}

//Remove uma pizza do no folha
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

TNoInterno *retira_interno(TNoInterno *no, int pos){
    int i;
    for(i = pos; i < no->m - 1; i++){
        if(no->chaves[i+1]){
            no->chaves[i] = no->chaves[i+1];
        }
    }
    no->chaves[no->m-1] = -1;
    no->m--;
    return no;
}

//Move as pizzas
TNoFolha *shift(TNoFolha *no){
    int i;
    for(i = no->m; i>0; i--){
        no->pizzas[i] = no->pizzas[i-1];
    }
    return no;
}

//Acho que nao sera necessaria.
TNoInterno *shift_interno(TNoInterno *pai){//faz um shift
    //tenho q deslocar tanto os indices quanto
    //{1,5,9}
    //{n,(2,3,4), (6,7,8), (10,11,23)}
    if(pai){
        int i;
        for(i = 0; i < pai->m; i++){
            if(i >= pai->m-1){//pra nao dar segfault
                pai->p[i] = pai->p[i+1];
            }
            else{
                pai->chaves[i] = pai->chaves[i+1];
                pai->p[i] = pai->p[i+1];
            }
        }
        //anulo a ultima posição q tinha valor
        int j = pai->m;
        while(j > pai->m-2){
            if(pai->p[j] != -1){
                pai->p[j] = -1;
                pai->chaves[j-1] = -1;
                break;
            }
            j++;
        }
        pai->m--;
        return pai;
    }return NULL;
}

TNoInterno * get_irmao_op_interno(int d,FILE *dados,TNoInterno *pai, TNoInterno *no, int pos, int *op, int *index){//retorna a posicao do irmao e a operação que será feita com ele
    // op = -1 = redist com o irmao da direita. op = 0 = redist com o filho da esquerda. op = 1 = concatenação com o filho da direita.
    int i = 0;
    int p = 0;//indice dos filhos

    if(pai){
        for(i = 0; i <= pai->m; i++){
            if(pai->p[i] == pos){
                *index = p = i;
                break;
            }
        }if(p == 0){
            fseek(dados, pai->p[p+1], SEEK_SET);
            TNoInterno *dir = le_no_interno(d, dados);
            if(dir){
                if(no->m + dir->m < 2*d){
                    *op = 1;
                }
                else{
                    *op = -1;
                }
            }
            return dir;
        }else{
            //carrego o no da direita e da esquerda
            TNoInterno *dir;
            TNoInterno *esq;
            fseek(dados, pai->p[p-1], SEEK_SET);
            esq = le_no_interno(2, dados);
            fseek(dados, pai->p[p+1], SEEK_SET);
            dir = le_no_interno(2, dados);
            //testo qual é o melhor e qual operacao
            if(dir){
                if(dir->m + no->m == 2*d){//redistribuição com o da direita
                    *op = -1;
                    return dir;
                }else if(!esq){//concatenação
                    *op = 1;
                    return dir;
                }
            }else if(esq){
                if(esq->m + no->m == 2*d){//redistribuição com o da esquerda
                    *op = 0;
                    return esq;
                }else{
                    *op = 2;
                    return esq;
                }
            }
        }
    }
    return NULL;
}

//Retorna o irmão e a operacao q tem q ser feita
TNoFolha * get_irmao_op(int d,FILE *dados,TNoInterno *pai, TNoFolha *no, int pos, int *op, int *index){//retorna a posicao do irmao e a operação que será feita com ele
    // op = -1 = redist com o irmao da direita. op = 0 = redist com o filho da esquerda. op = 1 = concatenação com o filho da direita.
    int i = 0;
    int p = 0;//indice dos filhos

    if(pai){
        for(i = 0; i <= pai->m; i++){
            if(pai->p[i] == pos){
                *index = p = i;
                break;
            }
        }
        if(p == 0){
            fseek(dados, pai->p[p+1], SEEK_SET);
            TNoFolha *dir = le_no_folha(d, dados);
            if(dir){
                if(no->m + dir->m < 2*d){
                    *op = 1;
                }
                else{
                    *op = -1;
                }
            }
            return dir;
        }else{
            //carrego o no da direita e da esquerda
            TNoFolha *dir;
            TNoFolha *esq;
            fseek(dados, pai->p[p-1], SEEK_SET);
            esq = le_no_folha(2, dados);
            fseek(dados, pai->p[p+1], SEEK_SET);
            dir = le_no_folha(2, dados);
            //testo qual é o melhor e qual operacao
            if(dir){
                if(dir->m + no->m == 2*d){//redistribuição com o da direita
                    *op = -1;
                    return dir;
                }else if(!esq){//concatenação
                    *op = 1;
                    return dir;
                }
            }else if(esq){
                if(esq->m + no->m == 2*d){//redistribuição com o da esquerda
                    *op = 0;
                    return esq;
                }else{
                    *op = 2;
                    return esq;
                }
            }
        }
    }return NULL;
}

void redistribui_dir_interno(TNoInterno *dir, TNoInterno *esq, TNoInterno *pai){
    int index = 0;
    busca_binaria(pai, &index, esq->chaves[0], 0, pai->m-1, 0);
    dir = shift_interno(dir);
    dir->chaves[0] = pai->chaves[index];
    pai->chaves[index] = esq->chaves[esq->m-1];
    dir->p[0] = esq->p[esq->m];
    esq->chaves[esq->m-1] = -1;
    esq->p[esq->m] = -1;
    esq->m--;
    dir->m++;
}
void redistribui_esq_interno(TNoInterno *dir, TNoInterno *esq, TNoInterno *pai){
    int index = 0;
    busca_binaria(pai, &index, esq->chaves[0], 0, pai->m-1, 0);
    esq->chaves[esq->m] = pai->chaves[index];
    pai->chaves[index] = dir->chaves[0];
    esq->p[esq->m+1] = dir->p[0];

    int i = 0;
    for(;i<dir->m - 1;i++){
        dir->chaves[i] = dir->chaves[i+1];
        dir->p[i] = dir->p[i+1];
    }
    dir->p[dir->m] = -1;
    dir->chaves[dir->m-1] = -1;
    esq->m++;
    dir->m--;
}
//Redistribui o irmão da direita com o da esquerda
void redistribui_dir(TNoFolha *dir, TNoFolha *esq, TNoInterno *pai){
    esq->pizzas[esq->m] = dir->pizzas[0];//passei do irmao da direita pro da esquerda;
    int cod = esq->pizzas[esq->m]->cod;
    dir = retira_pizza(dir, 0);//retirei o valor da direita e mantive salvo em cod
    int pos = 0;
    int ind = 0;
    ind = busca_binaria(pai, &pos, cod, 0, pai->m-1, 0);//achei a posição do pai que tem os filhos
    pai->chaves[pos] = dir->pizzas[0]->cod;//atualizei o indice
    esq->m++;
}


//Redistribui o irmão da esquerda pro da direita
void redistribui_esq(TNoFolha *dir, TNoFolha *esq, TNoInterno *pai){
    int cod = esq->pizzas[esq->m-1]->cod;
    dir = shift(dir);
    dir->pizzas[0] = esq->pizzas[esq->m-1];
    int pos = 0;
    busca_binaria(pai, &pos, cod, 0, pai->m-1, 0);//achei a posição do pai que tem os filhos
    pai->chaves[pos] = cod;//atualizei o indice
}


int q_filhos(TNoInterno *pai){
    if(pai){
        int i;
        int q = 1;
        for(i = 0; i < pai->m; i++){
            if(pai->p[i] != -1)q++;
        }
        return q;
    }
    return 0;
}

//Exclui todas pizzas de uma categoria
void excluiCategoria(char *categoria, char *nome_arquivo_dados, char *nome_arquivo_indice, char *nome_arquivo_metadados, int d){

    TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);

    if(!metadados) return;//Caso nao tenha nada no metadados então a arvore está vazia

    if(metadados->raiz_folha) {

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
        fseek(arq_Dados, metadados->pont_raiz, SEEK_SET);
        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        int i = 0;

        while(i < noFolha->m) {
            if (strcmp(noFolha->pizzas[i]->categoria, categoria) == 0) {
                exclui(noFolha->pizzas[i]->cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

            } else {
                i++;
            }
            fseek(arq_Dados, metadados->pont_raiz, SEEK_SET);
            noFolha = le_no_folha(d, arq_Dados);
        }


        if(noFolha->m ==0) fclose(fopen(nome_arquivo_metadados, "wb"));

        fclose(arq_Dados);

    }else{
        FILE *arq_Indice = fopen(nome_arquivo_indice, "rb");
        fseek(arq_Indice, metadados->pont_raiz, SEEK_SET);
        TNoInterno *noInterno = le_no_interno(d, arq_Indice);

        while(!noInterno->aponta_folha){
            fseek(arq_Indice, noInterno->p[0], SEEK_SET);
            noInterno = le_no_interno(d, arq_Indice);
        }
        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");

        fseek(arq_Dados, noInterno->p[0] ,SEEK_SET);
        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        int i = 0;
        while(noFolha) {
            while(i < noFolha->m) {
                if (strcmp(noFolha->pizzas[i]->categoria, categoria) == 0) {
                    exclui(noFolha->pizzas[i]->cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

                } else {
                    i++;
                }
                fseek(arq_Dados, metadados->pont_raiz, SEEK_SET);
                noFolha = le_no_folha(d, arq_Dados);
                i = 0;
            }
            if(noFolha->pont_prox == -1) break;
            fseek(arq_Dados, noFolha->pont_prox, SEEK_SET);

            noFolha = le_no_folha(d, arq_Dados);
        }

        fclose(arq_Dados);

    }

}

void concatena_interno(TNoInterno *w, TNoInterno *irmao, TNoInterno *pai, int d, int op, int index, FILE *dados, FILE *indice){
    int p_irmao = 0;
    if(op == DIR) {
        busca_binaria(pai, &p_irmao, irmao->chaves[0], 0, irmao->m - 1, 0);
        index = p_irmao - 1;

        w->chaves[w->m] = pai->chaves[index];
        w->p[w->m + 1] = irmao->p[0];
        w->m++;
        if(w->aponta_folha) {//Altera o ponteiro do no pai nos noh folha
            fseek(dados, irmao->p[0], SEEK_SET);
            TNoFolha *filho = le_no_folha(d, dados);
            filho->pont_pai = pai->p[index];
            fseek(dados, irmao->p[0], SEEK_SET);
            salva_no_folha(d, filho, dados);//salva noh com o valor do pai alterado
            for(int i = index + 1; i<= irmao->m ;i++){
                fseek(dados, irmao->p[i], SEEK_SET);
                filho = le_no_folha(d, dados);
                filho->pont_pai = pai->p[index];
                fseek(dados, irmao->p[i], SEEK_SET);
                salva_no_folha(d, filho, dados);
            }
        }else{//Altera o ponteiro pro pai
            fseek(indice, irmao->p[0], SEEK_SET);
            TNoInterno *filho = le_no_interno(d, indice);
            filho->pont_pai = pai->p[index];
            fseek(dados, irmao->p[0], SEEK_SET);
            salva_no_interno(d, filho, indice);//salva noh com o valor do pai alterado
            for(int i = index + 1; i<= irmao->m ;i++){
                fseek(dados, irmao->p[i], SEEK_SET);
                filho = le_no_interno(d, indice);
                filho->pont_pai = pai->p[index];
                fseek(dados, irmao->p[i], SEEK_SET);
                salva_no_interno(d, filho, indice);
            }

        }
        for (int i = 0; i < irmao->m; i++){
            w->chaves[w->m] = irmao->chaves[i];
            w->p[w->m + 1] = irmao->p[i + 1];
            w->m++;
        }
        for (int i = index; i < pai->m - 1; i++) {

            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1) //
                pai->p[i + 1] = pai->p[i + 2];
        }
        //Garante que a ultima posição será -1
        pai->chaves[index] = -1;
        pai->p[pai->m] = -1;
        pai->m--;

    }else if(op == ESQ){
        busca_binaria(pai, &p_irmao, irmao->chaves[0], 0, pai->m, 0);
        index = p_irmao;

        irmao = shift_interno(irmao);
        irmao->chaves[0] = pai->chaves[index];
        irmao->p[0] = w->p[w->m];
        irmao->m++;
        if(irmao->aponta_folha) {
            fseek(dados, w->p[w->m], SEEK_SET);
            TNoFolha *filho = le_no_folha(d, dados);
            filho->pont_pai = pai->p[index];
            salva_no_folha(d, filho, dados);
            for(int i = index + 1; i< irmao->m ;i++){
                fseek(dados, w->p[i], SEEK_SET);
                filho = le_no_folha(d, dados);
                filho->pont_pai = pai->p[index];
                salva_no_folha(d, filho, dados);
            }

        }else{

        }

        for (int i =0; i < w->m; i++) {

            irmao->chaves[irmao->m] = w->chaves[i];
            irmao->p[irmao->m + 1] = w->p[i + 1];
            irmao->m++;
        }

        for (int i = index; i < pai->m; i++) {
            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1)
                pai->p[i + 1] = pai->p[i + 2];
        }

        //Garante que a ultima posição será -1
        if(pai->m == 2*d) pai->chaves[pai->m - 1] = -1;
        pai->p[pai->m] = -1;
        pai->m--;

    }

}

void concatena(TNoFolha *no, TNoFolha *irmao, TNoInterno *pai, int d, int op, int ind_pai ,int *index, FILE *dados){
    if(op == DIR) {
        for (int i =0; i < irmao->m; i++){
            no->pizzas[no->m] = irmao->pizzas[i];
            no->m++;
        }

        //*index = 0; //Para descobrir qual index do pai vai ser apagado
        //for(int i = 1; pai->p[i] != no->pont_prox; i++ ) (*index)++;
        busca_binaria(pai, index, irmao->pizzas[0]->cod, 0, irmao->m -1, 0);
        no->pont_prox = irmao->pont_prox;
        for (int i = *index; i < pai->m; i++) {

            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1) // Se a nova chave for diferente de -1 ele leva o ponteiro do irmao
                pai->p[i + 1] = irmao->pont_prox;
            fseek(dados, irmao->pont_prox, SEEK_SET);
            irmao = le_no_folha(d, dados);
        }
        //Garante que a ultima posição será -1
        if(pai->m == 2*d) pai->chaves[pai->m - 1] = -1;
        pai->p[pai->m] = -1;
        pai->m--;

    }else if(op == ESQ){
        for (int i =0; i < no->m; i++) {

            irmao->pizzas[irmao->m] = no->pizzas[i];
            irmao->m++;
        }

        //*index = 0; //Para descobrir qual index do pai vai ser apagado
        //for(int i = 1; pai->p[i] != irmao->pont_prox; i++ ) (*index)++;
        busca_binaria(pai, index, irmao->pizzas[0]->cod, 0, irmao->m -1, 0);

        irmao->pont_prox = no->pont_prox; //Aponta para onde o noh excluido apontava

        for (int i = *index; i < pai->m; i++) {
            pai->chaves[i] = pai->chaves[i + 1];
            if(pai->chaves[i] != -1)// Se a nova chave for diferente de -1 ele leva o ponteiro do irmao
                pai->p[i + 1] = no->pont_prox;
            fseek(dados, no->pont_prox, SEEK_SET);
            no = le_no_folha(d, dados);
        }

        //Garante que a ultima posição será -1
        if(pai->m == 2*d) pai->chaves[pai->m - 1] = -1;
        pai->p[pai->m] = -1;
        pai->m--;

    }
}

void exclui_interno(int d, FILE *indice, TNoInterno *w, TNoInterno *pai, TNoInterno *irmao, int op, int index, char *nome_arquivo_metadados, FILE *dados){

    if(op == -1){//redistribui com o da direita
        redistribui_dir_interno(irmao, w, pai);

        fseek(indice, pai->p[index], SEEK_SET);//vou gravar o noh p
        salva_no_interno(d, w, indice);
        fseek(indice, pai->p[index + 1], SEEK_SET);
        salva_no_interno(d, irmao, indice);
        fseek(indice, w->pont_pai, SEEK_SET);
        salva_no_interno(d, w, indice);

    }
    else if(op == 0){//redistribui com o da esquerda
        redistribui_esq_interno(irmao, w, pai);

        fseek(indice, pai->p[index], SEEK_SET);//vou gravar o noh p
        salva_no_interno(d, w, indice);
        fseek(indice, pai->p[index + 1], SEEK_SET);
        salva_no_interno(d, irmao, indice);
        fseek(indice, w->pont_pai, SEEK_SET);
        salva_no_interno(d, w, indice);


    }else{//concatena

        concatena_interno(w, irmao, pai, d, op, index, dados, indice);

        if(pai->pont_pai == -1 && q_filhos(pai) == 1){//vai ter q apagar esse pai

            TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);

            irmao->pont_pai = -1;
            w->pont_pai = -1;
            metadados->pont_raiz = pai->p[0];//vai ser sempre essa posição (graças a deus)
            salva_arq_metadados(nome_arquivo_metadados, metadados);
            fseek(indice, pai->p[0], SEEK_SET);
            if(op == DIR)
                salva_no_interno(d, w, indice);
            else if (op == ESQ)
                salva_no_interno(d, irmao, indice);

        }else if(pai->m < d && pai->pont_pai != -1){//vai propagar. pode ser redistribuição de noh interno ou
            int i;//Acha o indice
            fseek(indice, pai->pont_pai, SEEK_SET);
            TNoInterno *p = le_no_interno(d, indice);
            TNoInterno *irmao_interno = get_irmao_op_interno(d, indice, p, pai, w->pont_pai, &op, &i);
            exclui_interno(d, indice, pai, p, irmao_interno, op, i, nome_arquivo_metadados, dados);
            fseek(indice, pai->p[index], SEEK_SET);

        }else{
            //salvo na posicao q o pai já estava
            fseek(indice, irmao->pont_pai, SEEK_SET);
            salva_no_interno(d, pai, indice);
            fseek(indice, pai->p[index], SEEK_SET);

            if(op == DIR)
                salva_no_interno(d, w, indice);
            else if (op == ESQ)
                salva_no_interno(d, irmao, indice);

        }


    }
}

int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
    TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);
    if(buscaFolha(pos, nome_arquivo_dados, cod, d)){
        FILE * dados = fopen(nome_arquivo_dados, "rb+");

        if(!dados)return -1;
        FILE *indice = fopen(nome_arquivo_indice, "rb+");
        fseek(dados, pos, SEEK_SET);
        TNoFolha * no = le_no_folha(d, dados);

        int p = 0;
        busca_binaria(no, &p, cod, 0, no->m, 1);

        if(no->m == 1){

            no = retira_pizza(no, p);

            fseek(dados, pos, SEEK_SET);
            salva_no_folha(d, no, dados);

            libera_no_folha(d, no);

            metadados->pont_raiz = -1;
            fseek(dados, 0, SEEK_END);
            metadados->pont_prox_no_folha_livre = ftell(dados);
            fseek(indice, 0, SEEK_END);
            metadados->pont_prox_no_interno_livre = ftell(indice);
            salva_arq_metadados(nome_arquivo_metadados, metadados);
            fclose(indice);
            fclose(dados);

        }else if(no->m > d || no->pont_pai == -1){//nao tem concatenacao nem redistribuicao

            no = retira_pizza(no, p);

            fseek(dados, pos, SEEK_SET);
            salva_no_folha(d, no, dados);

            libera_no_folha(d, no);
            fclose(dados);

        }else{//caso de haver redistribuição ou concatenação

            fseek(indice, no->pont_pai, SEEK_SET);
            TNoInterno *w = le_no_interno(d, indice);
            if(!w)return -1;
            int op = -2;
            int index = 0;
            no = retira_pizza(no, p);
            TNoFolha *irmao = get_irmao_op(d, dados, w, no, pos, &op, &index);

            if(op == -1){
                redistribui_dir(irmao, no, w);

                fseek(dados, w->p[index], SEEK_SET);//vou gravar o noh p
                salva_no_folha(d, no, dados);
                fseek(dados, w->p[index + 1], SEEK_SET);
                salva_no_folha(d, irmao, dados);
                fseek(indice, no->pont_pai, SEEK_SET);
                salva_no_interno(d, w, indice);
                fclose(indice);
                fclose(dados);
            }
            else if(op == 0){
                redistribui_esq(irmao, no, w);

                fseek(dados, w->p[index], SEEK_SET);//vou gravar o noh p
                salva_no_folha(d, no, dados);
                fseek(dados, w->p[index + 1], SEEK_SET);
                salva_no_folha(d, irmao, dados);
                fseek(indice, no->pont_pai, SEEK_SET);
                salva_no_interno(d, w, indice);
                fclose(indice);

                fclose(dados);

            }else{//concatena

                int guardaP;
                concatena(no, irmao, w, d, op, no->pont_pai, &guardaP, dados);

                if(w->pont_pai == -1 && q_filhos(w) == 1){//vai ter q apagar esse pai

                    irmao->pont_pai = -1;
                    metadados->pont_raiz = w->p[0];//vai ser sempre essa posição (graças a deus)
                    metadados->raiz_folha = 1;
                    salva_arq_metadados(nome_arquivo_metadados, metadados);
                    pos = w->p[0];
                    fseek(dados, pos, SEEK_SET);
                    salva_no_folha(d, irmao, dados);
                    fclose(dados);
                    libera_no_interno(w);
                    libera_no_folha(d, no);

                }else if(w->m < d && w->pont_pai != -1){//vai propagar. pode ser redistribuição de noh interno ou
                    fseek(indice, w->pont_pai, SEEK_SET);
                    TNoInterno *pai = le_no_interno(d, indice);
                    TNoInterno *irmao_interno = get_irmao_op_interno(d, indice, pai, w, no->pont_pai, &op, &index);
                    exclui_interno(d, indice, w, pai, irmao_interno, op, index, nome_arquivo_metadados, dados);

                    fseek(dados, w->p[guardaP], SEEK_SET);

                    if(op == DIR)
                        salva_no_folha(d, no, dados);
                    else if (op == ESQ)
                        salva_no_folha(d, irmao, dados);

                    fclose(indice);
                    fclose(dados);

                }else{//salvo na posicao q o pai já estava

                    fseek(indice, irmao->pont_pai, SEEK_SET);
                    salva_no_interno(d, w, indice);

                    fseek(dados, w->p[guardaP], SEEK_SET);

                    if(op == DIR)
                        salva_no_folha(d, no, dados);
                    else if (op == ESQ)
                        salva_no_folha(d, irmao, dados);

                    fclose(indice);
                    fclose(dados);

                }

            }
        }
        return pos;
    }else return -1;
}

void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados) {

    TListaPizzas *pizzas;
    fclose(fopen(nome_arquivo_dados, "wb"));//Apaga o conteudo do arquivo.
    fclose(fopen(nome_arquivo_indice, "wb"));
    fclose(fopen(nome_arquivo_metadados, "wb"));
    pizzas = le_pizzas(nome_arquivo_entrada);

    for (int i = 0; i < pizzas->qtd; i++) {

        insere(pizzas->lista[i]->cod,
               pizzas->lista[i]->nome,
               pizzas->lista[i]->categoria,
               pizzas->lista[i]->preco,
               nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d
        );


    }

}

//Imprime as pizzas
int imprimirArvore(int d, char *nome_arquivo_dados, char *nome_arquivo_indice, char *nome_arquivo_metadados) {

    TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);

    if (!metadados) return 0;//Caso nao tenha nada no metadados então a arvore está vazia


    if (metadados->pont_raiz == -1){
        return 0;
    }
    if(metadados->raiz_folha){

        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");
        fseek(arq_Dados, metadados->pont_raiz, SEEK_SET);
        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        imprime_no_folha(d, noFolha);
        return 1;
    }else {
        FILE *arq_Indice = fopen(nome_arquivo_indice, "rb");

        fseek(arq_Indice, metadados->pont_raiz, SEEK_SET);
        TNoInterno *noInterno = le_no_interno(d, arq_Indice);

        while(!noInterno->aponta_folha){
            fseek(arq_Indice, noInterno->p[0], SEEK_SET);
            noInterno = le_no_interno(d, arq_Indice);
        }
        FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");

        fseek(arq_Dados, noInterno->p[0] ,SEEK_SET);

        TNoFolha *noFolha = le_no_folha(d, arq_Dados);
        if (noFolha) {
            imprime_no_folha(d, noFolha);
            while (noFolha->pont_prox != -1) {
                fseek(arq_Dados, noFolha->pont_prox, SEEK_SET);
                noFolha = le_no_folha(d, arq_Dados);
                imprime_no_folha(d, noFolha);
            }
            return 1;
        } else return 0;
    }

}

//Verifica se o codigo da pizza existe
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

//Modifica a pizza
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
    printf("3. Remover todas pizzas de uma categoria.\n");
    printf("4. Imprimir arvore.\n");
    printf("5. Buscar pizza.\n");
    printf("6. Buscar todas pizzas de uma categoria.\n");
    printf("7. Carregar pizzas apartir de um arquivo.\n");
    printf("8. Modificar pizza.\n");
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

        int cod;
        printf("Codigo:\n");
        scanf("%d", &cod);
        exclui(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    }else if(opt == 3){//Remove todas as pizzas de uma categoria
        char categoria[20];

        printf("Digite a categoria:\n");
        scanf(" %[^\n]", categoria);

        excluiCategoria(categoria, nome_arquivo_dados, nome_arquivo_indice, nome_arquivo_metadados, d);

    }else if(opt == 4){//Imprimir arvore

        if(!imprimirArvore(d, nome_arquivo_dados,nome_arquivo_indice, nome_arquivo_metadados)) printf("Sem pizzas cadastradas.\n");

    }else if(opt == 5){//Buscar pizza

        int cod;
        printf("Codigo:\n");
        scanf("%d", &cod);
        if(!buscaPizza(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d))
            printf("Pizza nao existente.\n");

    }else if(opt == 6){//Buscar todas as pizzas de uma categoria

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

    }else if(opt == 7){//Carregar pizzas a partir de um arquivo

        char nome_arquivo_entrada[20];

        printf("Nome do arquivo de entrada(com .txt ou .dat).\n");
        scanf(" %s", nome_arquivo_entrada);
        carrega_dados(d, nome_arquivo_entrada, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados);

    }else if(opt == 8){//modificar pizza.

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
    char nome_arquivo_metadados[20];// = "metadados.dat";
    char nome_arquivo_indice[20];// = "indice.dat";
    char nome_arquivo_dados[20];// = "pizzas.dat";
    printf("Digite a ordem da arvore.\n");
    scanf(" %d", &d);
    printf("Nome do arquivo de metadados(com .txt ou .dat).\n");
    scanf(" %s", nome_arquivo_metadados);
    printf("Nome do arquivo de indice(com .txt ou .dat).\n");
    scanf(" %s", nome_arquivo_indice);
    printf("Nome do arquivo de dados(com .txt ou .dat).\n");
    scanf(" %s", nome_arquivo_dados);

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
