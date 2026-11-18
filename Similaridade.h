#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "leitura.h"
#include "alocar_matriz.h"

#include <iostream>
#include <vector>
#include<map>
#include <string>
#include <list>

int** calcula_intercecao(const list<vector<int>>& lista_compra, int ncliente, int nproduto);
int** calcula_intercecao_otimizada(const list<vector<int>>& lista_compra, int ncliente, int nproduto);
void calcularJaccard(int** I, const list<vector<int>>& listaCompra, double** S, int nCliente);
void calcularSimilaridade(const list<vector<int>>& listaCompra, double** S, int nCliente, int nProd);
void calcularSimilaridadeOtimizada(const list<vector<int>>& listaCompra, double** S, int nCliente, int nProd);
int similaridade(listaCompras &lista);
#endif