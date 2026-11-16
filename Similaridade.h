#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "leitura.h"
#include "alocar_matriz.h"

#include <iostream>
#include <vector>
#include<map>
#include <string>

int** calcula_intercecao(const vector<vector<int>>& lista_compra, int ncliente, int nproduto);
void calcularJaccard(int** I, const vector<vector<int>>& listaCompra, double** S, int nCliente);
void calcularSimilaridade(const vector<vector<int>>& listaCompra, double** S, int nCliente, int nProd);
int encontrarClienteMaisSimilar(double** S, int ncliente, int cliente_alvo);
int similaridade(listaCompras &lista);

#endif