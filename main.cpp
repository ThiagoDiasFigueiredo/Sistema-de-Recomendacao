#include <stdio.h>
#include "leitura.h"
#include "similaridade.h"
#include "recomendacao.h"
int main(int argc, char *argv[]) {
    listaCompras lista;

    leitura(argc,argv,lista);
    similaridade(lista);
    
    return 0;
}