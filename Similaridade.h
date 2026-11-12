#include <stdio.h>
#include <stdlib.h>

typedef struct NO {
    int id_produto;
    struct NO* proximo;
} NO;


int** alocarMatrizInt(int linhas, int colunas) {
    int** matriz = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int*)malloc(colunas * sizeof(int));

        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

double** alocarMatrizDouble(int linhas, int colunas) {
    double** matriz = (double**)malloc(linhas * sizeof(double*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (double*)malloc(colunas * sizeof(double));

        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = 0.0;
        }
    }
    return matriz;
}

void liberarMatrizInt(int** matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void liberarMatrizDouble(double** matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int** calcula_intercecao(NO** lista_compra, int ncliente, int nproduto){
    int** matrizCompra = alocarMatrizInt(ncliente, nproduto);
    int** transposta = alocarMatrizInt(nproduto, ncliente);
    
    //preenche a matriz de compra de acordo com a lista da etapa 1
    for (int i = 0; i < ncliente; i++) {
        NO* atual = lista_compra[i];
        while (atual != NULL) {
            int id_produto = atual->id_produto;
            matrizCompra[i][id_produto] = 1;
            atual = atual->proximo;//pula para o proximo produto
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

void calcularJaccard(int** I, NO** listaCompra, double** S, int nCliente) {
    for (int i = 0; i < nCliente; i++) {
        // Conta quantos elementos existem na lista do cliente i (vinda da Etapa 1) para achar |P_i|
        int tamanho_Pi = 0;
        NO* atual = listaCompra[i];
        while (atual != NULL) {
            tamanho_Pi++;
            atual = atual->proximo;
        }
        
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


void calcularSimilaridade(NO** listaCompra, double** S, int nCliente, int nProd) {
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