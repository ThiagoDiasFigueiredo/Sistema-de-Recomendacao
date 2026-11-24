#ifndef CSR_H
#define CSR_H

#include "leitura.h"

typedef struct {
    int* valores;      // Valores nao nulos linha por linha
    int* col_index;   // Indice da coluna correspondente a cada valor em values
    int* linha_index;     // Indice de inicio de cada linha em values
    int linhas;
    int colunas;
    int total_elementos;          // Quantidade total de elementos nao nulos (cont)
} CSRInt;

// Estrutura CSR para matriz de ponto flutuante (Matriz de Similaridade S)
typedef struct {
    double* valores;   // Valores de similaridade
    int* col_index;   // Indice da coluna 
    int* linha_index;     // Indice de inicio de cada linha
    int linhas;
    int colunas;
    int total_elementos;          // Quantidade total de elementos armazenados
} CSRDouble;

// Funcoes de construcao, multiplicacao A * A^T e similaridade em CSR (Atividade 5)
CSRInt construirCSRCompras(const vector<list<int>>& lista_compra, int ncliente, int nproduto);
CSRInt multiplicarCSRTransposta(const CSRInt& A);
CSRDouble calcularJaccardCSR(const CSRInt& I, const CSRInt& A);
CSRDouble calcularSimilaridadeCSR(const vector<list<int>>& lista_compra, int ncliente, int nproduto);

// Funcoes utilitarias de consulta, memoria e liberacao
int encontrarClienteMaisSimilarCSR(const CSRDouble& S, int cliente_alvo, double* menor_dist);
size_t calcularMemoriaCSRInt(const CSRInt& M);
size_t calcularMemoriaCSRDouble(const CSRDouble& M);
void liberarCSRInt(CSRInt* M);
void liberarCSRDouble(CSRDouble* M);

// Testador da Etapa 5 (compara tempo e memoria: Densa vs CSR)
int similaridadeCSR(listaCompras &lista);

void calcularSimilaridadeOtimizada(const list<vector<int>>& listaCompra, double** S, int nCliente, int nProd);
int encontrarClienteMaisSimilar(double** S, int ncliente, int cliente_alvo);

#endif