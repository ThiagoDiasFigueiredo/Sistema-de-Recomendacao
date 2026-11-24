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
int identificarVizinhosCSR(const CSRDouble& S, int c, int* L, double* distL);

// Atividade 3.2
void inicializaRanqueamento(const vector<list<int>> &compras, int numProd, int c,
                            double* vetorR, int* marca, int* comprado);

// Atividade 3.3
void calcularRanqueamentoCSR(double* notaProduto, int* ultimoVizinho, const int* clienteComprou,
                             const vector<list<int>> &compras, const int* vizinhos,
                             const double* distVizinhos, int qtdVizinhos, int qtdProdutos);

// Atividade 3.4
int ordenaEseleciona(const double* vetorR, const int* marca, const int* comprado,
                     int nProd, int k, Ranking* resultado);

Ranking* recomendarCSR(const vector<list<int>> &compras, const CSRDouble& S,
                       int nProd, int c, int k, int* qtdRecomendados);

int recomendacao(listaCompras &lista, int cliente, int k);

#endif