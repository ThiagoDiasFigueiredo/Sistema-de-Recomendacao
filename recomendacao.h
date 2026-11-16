#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include "leitura.h"   


typedef struct
{
    int id; //ID interno do produto
    double valor; //nota R dele
} Ranking;

// Estado do ranqueamento dos produtos para um cliente c
typedef struct
{
    vector<double> vetorR;  // vetorR[p] - começa em 1
    vector<int> marca;      // marca[p] - último vizinho que atualizou p (-1 = nunca atualizado)
    vector<int> comprado;   // comprado[p] - 1 se o cliente c já comprou o produto p
} Ranqueamento;

vector<Ranking> recomendar(const vector<vector<int>> &compras, double** S,int nCliente, int nProd, int c, int k);

int recomendacao(listaCompras &lista, int cliente, int k);

#endif