#include <stdio.h>
#include "leitura.h"
#include "similaridade.h"
#include "recomendacao.h"
int main(int argc, char *argv[]) {
    listaCompras lista;

    leitura(argc,argv,lista);
    similaridade(lista);
    recomendacao(lista,840,5); //O arquivo/Indice do cliente/Quantos produtos quer
    return 0;
}