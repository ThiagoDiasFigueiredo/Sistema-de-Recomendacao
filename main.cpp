#include <stdio.h>
#include "leitura.h"
#include "Similaridade.h"
#include "recomendacao.h"
#include "matrizes_esparsas.h"

int main(int argc, char *argv[]) {
    listaCompras lista;

    leitura(argc,argv,lista);
    similaridade(lista);
    similaridadeCSR(lista);
    recomendacao(lista,791,5); //O arquivo/Indice do cliente/Quantos produtos quer
    return 0;
}