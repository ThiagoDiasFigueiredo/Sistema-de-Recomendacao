#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include "leitura.h"

typedef struct
{
    int id;       // ID interno do produto
    double valor; // Nota R de ranqueamento
} Ranking;

bool comparaPorValor(const Ranking &a, const Ranking &b);

// Atividade 3.1
int indentificarVizinhos(double** S, int numCliente, int c, int* L);

// Atividade 3.2
void inicializaRanqueamento(const vector<vector<int>> &compras, int numProd, int c,
                            double* vetorR, int* marca, int* comprado);

// Atividade 3.3
void calcularRanqueamento(double* vetorR, int* marca, const int* comprado,
                          const vector<vector<int>> &compras, double** S,
                          const int* L, int tamL, int c, int numProd);

// Atividade 3.4
int ordenaEseleciona(const double* vetorR, const int* marca, const int* comprado,
                     int nProd, int k, Ranking* resultado);

Ranking* recomendar(const vector<vector<int>> &compras, double** S,
                    int nCliente, int nProd, int c, int k, int* qtdRecomendados);

int recomendacao(listaCompras &lista, int cliente, int k);

#endif