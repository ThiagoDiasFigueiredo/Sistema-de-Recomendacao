#include <stdio.h>
#include "leitura.h"
#include "Similaridade.h"

int main(int argc, char *argv[]) {
    // Cria a variável local para lista
    listaCompras lista;
    if (leitura(argc, argv, lista) != 0) {
        return 1;
    }

    // Descobre o número de clientes e de produtos
    int nCliente = lista.compras.size();
    int nProd = lista.codigo_produtos.size();

    // Aloca a matriz S e calcula a similaridade
    double** S = alocarMatrizDouble(nCliente, nCliente);
    calcularSimilaridade(lista.compras, S, nCliente, nProd);

    // Acha o cliente mais similar ao 0
    int cliente_alvo = 0;
    int mais_similar = encontrarClienteMaisSimilar(S, nCliente, cliente_alvo);

    printf("\n=== Resultado Etapa 2 (Similaridade) ===\n");
    if (mais_similar != -1) {
        printf("Cliente consultado: Indice %d (Codigo: %s)\n",
               cliente_alvo, lista.clientes[cliente_alvo].c_str());
        printf("Cliente mais similar: Indice %d (Codigo: %s)\n",
               mais_similar, lista.clientes[mais_similar].c_str());
        printf("Distancia de Jaccard s(i,j): %.4f\n", S[cliente_alvo][mais_similar]);
    }

    liberarMatrizDouble(S, nCliente);

    return 0;
}