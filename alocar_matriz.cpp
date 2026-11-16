#include "alocar_matriz.h"
#include <stdlib.h>

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