#include "Similaridade.h"
#include "leitura.h"
#include "alocar_matriz.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>


using namespace std;

int** calcula_intercecao(const vector<vector<int>>& lista_compra, int ncliente, int nproduto){
    int** matrizCompra = alocarMatrizInt(ncliente, nproduto);
    int** transposta = alocarMatrizInt(nproduto, ncliente);
    
    //preenche a matriz de compra de acordo com a lista da etapa 1

    for (int i = 0; i < ncliente; i++) {
        for (int j = 0; j < (int)lista_compra[i].size(); j++) {
            int id_produto = lista_compra[i][j];
            matrizCompra[i][id_produto] = 1;
        }
    }

    //Cria uma transposta
    for (int i = 0; i < ncliente; i++) {
        for (int j = 0; j < nproduto; j++) {
            transposta[j][i] = matrizCompra[i][j];
        }
    }

    //Cria uma matriz interceção
    int** I = alocarMatrizInt(ncliente, ncliente);
    for (int i = 0; i < ncliente; i++) {
        for (int j = 0; j < ncliente; j++) {
            I[i][j] = 0;
            for (int k = 0; k < nproduto; k++) {
                I[i][j] = I[i][j] + (matrizCompra[i][k] * transposta[k][j]);
            }
        }
    }
    liberarMatrizInt(matrizCompra, ncliente);
    liberarMatrizInt(transposta, nproduto);

    return I;
}

void calcularJaccard(int** I, const vector<vector<int>>& listaCompra, double** S, int nCliente) {
    for (int i = 0; i < nCliente; i++) {
        // Conta quantos elementos existem na lista do cliente i (vinda da Etapa 1) para achar |P_i|
        int tamanho_Pi = listaCompra[i].size();
        
        // Aplica a fórmula do quadro: S[i][j] = 1 - (I[i][j] / |P_i|)
        for (int j = 0; j < nCliente; j++) {
            if (tamanho_Pi > 0) {
                S[i][j] = 1.0 - ((double)I[i][j] / tamanho_Pi);
            } else {
                S[i][j] = 1.0;
            }
        }
    }
}


void calcularSimilaridade(const vector<vector<int>>& listaCompra, double** S, int nCliente, int nProd) {
    int** I = calcula_intercecao(listaCompra, nCliente, nProd);

    calcularJaccard(I, listaCompra, S, nCliente);

    liberarMatrizInt(I, nCliente);
}


int encontrarClienteMaisSimilar(double** S, int ncliente, int cliente_alvo) {
    int mais_similar = -1;
    double menor_distancia = 2.0;

    for (int j = 0; j < ncliente; j++) {
        if (j != cliente_alvo) {
            if (S[cliente_alvo][j] < menor_distancia) {
                menor_distancia = S[cliente_alvo][j];
                mais_similar = j;
            }
        }
    }
    return mais_similar;
}

int similaridade(listaCompras &lista){
    int nCliente = lista.compras.size();
    int nProd = lista.codigo_produtos.size();

    // Aloca a matriz S e calcula a similaridade
    double** S = alocarMatrizDouble(nCliente, nCliente);
    calcularSimilaridade(lista.compras, S, nCliente, nProd);

    // Acha o cliente mais similar ao c
    int cliente_alvo = 840;
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