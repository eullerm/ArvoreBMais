#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define DIR 1
#define  ESQ 0
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

//Insere um no folha
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
            //printf("Noh interno: COD %d\n", chave);

            TNoInterno *raiz = r;
            //imprime_no_interno(2, raiz);
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
        if(metadados->raiz_folha)return metadados->pont_raiz;
        else{
            fseek(indices, metadados->pont_raiz, SEEK_SET);
            raizI = le_no_interno(d, indices);
        }
    }

    if(!raizI && !raizF)return -1;

    int value = 0;
    int pos;
    int p_no = 0;

    while(raizI){//percorre o arquivo de indices
        pos = 0;
        value = busca_binaria(raizI, &pos, cod, 0, raizI->m-1, 0);
        //imprime_no_interno(2, raizI);

        if(value != cod && raizI->p[0] != -1){//se o elemento não está no nó e ele é interno
            if(raizI->chaves[pos] > cod){
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
        }else{//se o indice for igual a chave, eu tenho q carregar a folha
            fseek(indices, pos, SEEK_SET);
            raizI = le_no_interno(d, indices);
        }
    }
    return p_no;
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
/*
        rewind(arq_Dados);
        noFolha = le_no_folha(d, arq_Dados);
        while(noFolha){
            imprime_no_folha(d, noFolha);
            noFolha = le_no_folha(d, arq_Dados);
        }*/

        fclose(arq_Dados);

/*
        FILE *arqIndices = fopen(nome_arquivo_indice, "rb+");
        TNoInterno * no = le_no_interno(d, arqIndices);
        while(no) {
            imprime_no_interno(d, no);
            no = le_no_interno(d, arqIndices);
        }
        fclose(arqIndices);
*/
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

//Move as pizzas
TNoFolha *shift(TNoFolha *no){
    int i;
    for(i = no->m; i>0; i--){
        no->pizzas[i] = no->pizzas[i-1];
    }
    return no;
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

//Retorna o irmão
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
            //carrego o no da direita e da esquerda TODO nao aguento mais
            TNoFolha *dir;
            TNoFolha *esq;
            esq = dir = NULL;
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
                    *op = 1;
                    return esq;
                }
            }
        }
    }
    return NULL;
}

//Exclui todas pizzas de uma categoria
void excluiCategoria(char *categoria, char *nome_arquivo_dados, char *nome_arquivo_indice, char *nome_arquivo_metadados, int d){

    FILE *arq_Dados = fopen(nome_arquivo_dados, "rb");

    TNoFolha *noFolha = le_no_folha(d, arq_Dados);

    int i =0;

    while(noFolha){

        if(i < noFolha->m){
            if(strcmp(noFolha->pizzas[i]->categoria, categoria) == 0){
                exclui(noFolha->pizzas[i]->cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
            }i++;
        }else{
            noFolha = le_no_folha(d, arq_Dados);
            i = 0;
        }
    }
    fclose(arq_Dados);

}

void concatena(TNoFolha *no, TNoFolha *irmao, TNoInterno *pai, int d, int op){
    if(op == DIR) {
        for (int i = (no->m - 1); i < 0; i++){
            irmao = shift(irmao);
            irmao->pizzas[0] = no->pizzas[i];
            irmao->m++;

        }


    }
}

int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
    int pos = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

    if(buscaFolha(pos, nome_arquivo_dados, cod, d)){
        FILE * dados = fopen(nome_arquivo_dados, "rb+");

        if(!dados)return -1;
        FILE *indice = fopen(nome_arquivo_indice, "rb+");
        fseek(dados, pos, SEEK_SET);
        TNoFolha * no = le_no_folha(d, dados);

        int p = 0;
        busca_binaria(no, &p, cod, 0, no->m, 1);

        if(no->m > d || no->pont_pai == -1){//nao tem concatenacao nem redistribuicao

            no = retira_pizza(no, p);

            fseek(dados, pos, SEEK_SET);
            salva_no_folha(d, no, dados);

            /*rewind(dados);
            no = le_no_folha(d, dados);
            while(no){

                imprime_no_folha(d, no);
                no = le_no_folha(d, dados);
            }*/
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


                /*rewind(dados);
                no = le_no_folha(d, dados);
                while(no){

                    imprime_no_folha(d, no);
                    no = le_no_folha(d, dados);
                }

                imprime_no_interno(d, w);*/

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

                /*rewind(dados);
                no = le_no_folha(d, dados);
                while(no){

                    imprime_no_folha(d, no);
                    no = le_no_folha(d, dados);
                }*/

                fclose(dados);

            }
            else{//concatena

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
        //printf("Pizza %d\n", pizzas->lista[i]->cod);
        insere(pizzas->lista[i]->cod,
               pizzas->lista[i]->nome,
               pizzas->lista[i]->categoria,
               pizzas->lista[i]->preco,
               nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d
        );


    }

    //printf("\n\n");

}

//Imprime as pizzas
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

        if(!imprimirArvore(d, nome_arquivo_dados)) printf("Sem pizzas cadastradas.\n");

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

/*int main(){

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
}*/
