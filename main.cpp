#include "leitura.h"
#include "Similaridade.h"
#include "recomendacao.h"

int main(int argc, char *argv[]) {
    listaCompras lista;

    leitura(argc, argv, lista);
    similaridade(lista);
    recomendacao(lista, 791, 5); // O arquivo/Indice do cliente/Quantos produtos quer
    return 0;
}