#ifndef CSR_H
#define CSR_H

#include "leitura.h"
#include <vector>
#include <list>
#include <cstddef>
using namespace std;
typedef struct {
    vector<int> valores;      // Valores nao nulos linha por linha
    vector<int> col_index;    // Indice da coluna correspondente a cada valor
    vector<int> linha_index;  // Indice de inicio de cada linha
    int linhas;
    int colunas;
    int total_elementos;           // Quantidade total de elementos nao nulos
} CSRInt;

// Estrutura CSR para matriz de ponto flutuante (Matriz de Similaridade S)
typedef struct {
    vector<double> valores;   // Valores de similaridade
    vector<int> col_index;    // Indice da coluna
    vector<int> linha_index;  // Indice de inicio de cada linha
    int linhas;
    int colunas;
    int total_elementos;           // Quantidade total de elementos armazenados
} CSRDouble;

// Funcoes principais da Atividade 5
CSRInt construirCSRCompras(const std::vector<std::list<int>>& lista_compra, int ncliente, int nproduto);
CSRInt multiplicarCSRTransposta(const CSRInt& A);
CSRDouble calcularJaccardCSR(const CSRInt& I, const CSRInt& A);
CSRDouble calcularSimilaridadeCSR(const std::vector<std::list<int>>& lista_compra, int ncliente, int nproduto);

// Funcoes utilitarias e testador da Atividade 5
int encontrarClienteMaisSimilarCSR(const CSRDouble& S, int cliente_alvo, double& menor_dist);
size_t calcularMemoriaCSRInt(const CSRInt& M);
size_t calcularMemoriaCSRDouble(const CSRDouble& M);
int similaridadeCSR(listaCompras &lista);

#endif