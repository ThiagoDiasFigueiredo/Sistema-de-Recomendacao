#include "Similaridade.h"
#include "leitura.h"
#include "alocar_matriz.h"
#include "matrizes_esparsas.h"

#include <stdio.h>
#include <vector>
#include <ctime>
#include <list>
using namespace std;

int** calcula_intercecao(const vector<list<int>>& lista_compra, int ncliente, int nproduto){
    int** matrizCompra = alocarMatrizInt(ncliente, nproduto);
    int** transposta = alocarMatrizInt(nproduto, ncliente);
    
    //preenche a matriz de compra de acordo com a lista da etapa 1
    for (int i = 0; i < ncliente && i < (int)lista_compra.size(); i++) {
        for (auto it = lista_compra[i].begin(); it != lista_compra[i].end(); ++it) {
            int id_produto = *it;
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

int** calcula_intercecao_otimizada(const vector<list<int>>& lista_compra, int ncliente, int nproduto) {
    int** matrizCompra = alocarMatrizInt(ncliente, nproduto);

    for (int i = 0; i < ncliente && i < (int)lista_compra.size(); i++) {
        for (auto it = lista_compra[i].begin(); it != lista_compra[i].end(); ++it) {
            int id_produto = *it;
            if (id_produto >= 0 && id_produto < nproduto) {
                matrizCompra[i][id_produto] = 1;
            }
        }
    }

    int** I = alocarMatrizInt(ncliente, ncliente);

    for (int i = 0; i < ncliente; i++) {
        for (int j = i; j < ncliente; j++) {
            int soma = 0;
            for (int k = 0; k < nproduto; k++) {
                soma = soma + (matrizCompra[i][k] * matrizCompra[j][k]);
            }
            I[i][j] = soma;
            I[j][i] = soma;
        }
    }

    liberarMatrizInt(matrizCompra, ncliente);
    return I;
}

void calcularJaccard(int** I, const vector<list<int>>& listaCompra, double** S, int nCliente)  {
    for (int i = 0; i < nCliente && i < (int)listaCompra.size(); i++) {
        // Conta quantos elementos existem na lista do cliente i (vinda da Etapa 1) para achar |P_i|
        int tamanho_Pi = (int)listaCompra[i].size();
        
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


void calcularSimilaridade(const vector<list<int>>& listaCompra, double** S, int nCliente, int nProd) {
    int** I = calcula_intercecao(listaCompra, nCliente, nProd);

    calcularJaccard(I, listaCompra, S, nCliente);

    liberarMatrizInt(I, nCliente);
}

void calcularSimilaridadeOtimizada(const vector<list<int>>& listaCompra, double** S, int nCliente, int nProd) {
    int** I = calcula_intercecao_otimizada(listaCompra, nCliente, nProd);

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
    int nCliente = (int)lista.compras.size();
    int nProd = (int)lista.codigo_produtos.size();
    int cliente_alvo =791;

    if (nCliente == 0) {
        printf("Nenhum cliente carregado.\n");
        return 1;
    }

    int usuario;
    printf("\nDigite uma opcao:\n");
    printf("[1] Similaridade pela transposta (Densa - Atividade 2)\n");
    printf("[2] Similaridade pelo metodo do somatorio (Densa - Atividade 4)\n");
    printf("[3] Similaridade por Matriz Esparsa CSR (Atividade 5)\n");
    scanf("%d",&usuario);
    if(usuario == 1){

        clock_t inicio = clock();
        // Aloca a matriz S e calcula a similaridade
        double** S = alocarMatrizDouble(nCliente, nCliente);
        calcularSimilaridade(lista.compras, S, nCliente, nProd);

        // Acha o cliente mais similar ao c
        int mais_similar = encontrarClienteMaisSimilar(S, nCliente, cliente_alvo);
        clock_t fim = clock();
        printf("=== Resultado Etapa 2 (Similaridade) ===\n");
        printf("===== PRIMEIRO METODO =======\n");
        if (mais_similar != -1) {
            printf("Cliente consultado: Indice %d (Codigo: %s)\n",
                cliente_alvo, lista.clientes[cliente_alvo].c_str());
            printf("Cliente mais similar: Indice %d (Codigo: %s)\n",
                mais_similar, lista.clientes[mais_similar].c_str());
            printf("Distancia de Jaccard s(i,j): %.4f\n", S[cliente_alvo][mais_similar]);
            double tempo = double(fim - inicio) / CLOCKS_PER_SEC;
            printf("Tempo em segundos: %.4f\n",tempo);
        }

        liberarMatrizDouble(S, nCliente);
    } else if (usuario == 2){

        clock_t inicio = clock();

        // Aloca a matriz S e calcula a similaridade
        double** S = alocarMatrizDouble(nCliente, nCliente);
        calcularSimilaridadeOtimizada(lista.compras, S, nCliente, nProd);

        // Acha o cliente mais similar ao c
        int mais_similar = encontrarClienteMaisSimilar(S, nCliente, cliente_alvo);
        clock_t fim = clock();
        printf("\n=== Resultado Etapa 2 (Similaridade) ===\n");
        printf("===== SEGUNDO METODO =======");
        if (mais_similar != -1) {
            printf("Cliente consultado: Indice %d (Codigo: %s)\n",
                cliente_alvo, lista.clientes[cliente_alvo].c_str());
            printf("Cliente mais similar: Indice %d (Codigo: %s)\n",
                mais_similar, lista.clientes[mais_similar].c_str());
            printf("Distancia de Jaccard s(i,j): %.4f\n", S[cliente_alvo][mais_similar]);
            double tempo = double(fim - inicio) / CLOCKS_PER_SEC;
            printf("Tempo em segundos: %.4f\n",tempo);
        }

        liberarMatrizDouble(S, nCliente);
    }

else if (usuario == 3) {
        similaridadeCSR(lista);
    }


    return 0;
}