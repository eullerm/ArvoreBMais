#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string.h>
#include <stdlib.h>

#include "metadados.h"
#include "no_folha.h"


void imprime_metadados(TMetadados *metadados)
{
	printf("%d, %d, %d, %d, %d\n", metadados->d, metadados->pont_raiz, metadados->raiz_folha,
		metadados->pont_prox_no_interno_livre, metadados->pont_prox_no_folha_livre);
}

TMetadados *metadados(int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre)
{
	TMetadados *metadados = (TMetadados *) malloc(sizeof(TMetadados));
	if (metadados) memset(metadados, 0, sizeof(TMetadados));
	metadados->d = d;
	metadados->pont_raiz = pont_raiz;
	metadados->raiz_folha = raiz_folha;
	metadados->pont_prox_no_interno_livre = pont_prox_no_interno_livre;
	metadados->pont_prox_no_folha_livre = pont_prox_no_folha_livre;
	return metadados;
}

TMetadados *metadados_vazio(int d)
{
	return metadados(d, 0, 1, 0, tamanho_no_folha(d));
}

void salva_metadados(TMetadados *metadados, FILE *out)
{
	fwrite(&metadados->d, sizeof(int), 1, out);
	fwrite(&metadados->pont_raiz, sizeof(int), 1, out);
	fwrite(&metadados->raiz_folha, sizeof(int), 1, out);
	fwrite(&metadados->pont_prox_no_interno_livre, sizeof(int), 1, out);
	fwrite(&metadados->pont_prox_no_folha_livre, sizeof(int), 1, out);
}

void salva_arq_metadados(char *nome_arquivo, TMetadados *metadados)
{
	FILE *out = fopen(nome_arquivo, "wb");
	salva_metadados(metadados, out);
	fclose(out);
}


TMetadados *le_metadados(FILE *in)
{
	TMetadados *metadados = (TMetadados *) malloc(sizeof(TMetadados));
	if (0 >= fread(&metadados->d, sizeof(int), 1, in)) {
		free(metadados);
		return NULL;
	}
	fread(&metadados->pont_raiz, sizeof(int), 1, in);
	fread(&metadados->raiz_folha, sizeof(int), 1, in);
	fread(&metadados->pont_prox_no_interno_livre, sizeof(int), 1, in);
	fread(&metadados->pont_prox_no_folha_livre, sizeof(int), 1, in);
	return metadados;
}

TMetadados *le_arq_metadados(char *nome_arquivo)
{
	FILE *in = fopen(nome_arquivo, "rb");
	if (in != NULL) {
		TMetadados *resultado = le_metadados(in);
		fclose(in);
		return resultado;
	}
	else return NULL;
}


int cmp_metadados(int d, TMetadados *m1, TMetadados *m2)
{
	if (m1 == NULL) {
		return (m2 == NULL);
	}
	if (m2 == NULL) {
		return 0;
	}
	if (m1->pont_raiz != m2->pont_raiz) {
		return 0;
	}
	if (m1->raiz_folha != m2->raiz_folha) {
		return 0;
	}
	if (m1->pont_prox_no_interno_livre != m2->pont_prox_no_interno_livre) {
		return 0;
	}
	if (m1->pont_prox_no_folha_livre != m2->pont_prox_no_folha_livre) {
		return 0;
	}
	return 1;
}

int tamanho_metadados()
{
	return sizeof(int) + //d
	    sizeof(int) + // pont_raiz
		sizeof(int) + // raiz_folha
		sizeof(int) + // pont_prox_no_interno_livre
		sizeof(int); // pont_prox_no_folha_livre
}