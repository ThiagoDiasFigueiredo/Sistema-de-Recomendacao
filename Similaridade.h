#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "leitura.h"
#include "alocar_matriz.h"

#include <iostream>
#include <vector>
#include<map>
#include <string>
#include <list>

int** calcula_intercecao(const vector<list<int>>& lista_compra, int ncliente, int nproduto);
int** calcula_intercecao_otimizada(const vector<list<int>>& lista_compra, int ncliente, int nproduto);
void calcularJaccard(int** I, const vector<list<int>>& listaCompra, double** S, int nCliente);
void calcularSimilaridade(const vector<list<int>>& listaCompra, double** S, int nCliente, int nProd);
void calcularSimilaridadeOtimizada(const vector<list<int>>& listaCompra, double** S, int nCliente, int nProd);
int similaridade(listaCompras &lista);
int encontrarClienteMaisSimilar(double** S, int ncliente, int cliente_alvo);
#endif