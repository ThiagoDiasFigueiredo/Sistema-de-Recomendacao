#include "matrizes_esparsas.h"
#include "Similaridade.h"
#include "alocar_matriz.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>

using namespace std;

// Converte a lista de compras (Etapa 1) para a Matriz de Compras A no formato CSR
CSRInt construirCSRCompras(const list<vector<int>>& lista_compra, int ncliente, int nproduto) {
    CSRInt A;
    A.linhas = ncliente;
    A.colunas = nproduto;

    // Conta o total de elementos nao nulos (compras unicas)
    int total_elementos = 0;
    for (auto it = lista_compra.begin(); it != lista_compra.end(); ++it) {
        total_elementos += (int)it->size();
    }
    A.total_elementos = total_elementos;

    A.valores = (int*)malloc(total_elementos * sizeof(int));
    A.col_index = (int*)malloc(total_elementos * sizeof(int));
    A.linha_index = (int*)malloc((ncliente + 1) * sizeof(int));

    int cont = 0;
    int i = 0;
    for (auto it = lista_compra.begin(); it != lista_compra.end() && i < ncliente; ++it, i++) {
        A.linha_index[i] = cont;
        int tam = (int)it->size();

        // Copia os produtos comprados pelo cliente i
        for (int j = 0; j < tam; j++) {
            A.col_index[cont + j] = (*it)[j];
        }

        // Ordena os indices de coluna da linha i para permitir a intersecao em CSR
        sort(A.col_index + cont, A.col_index + cont + tam);

        for (int j = 0; j < tam; j++) {
            A.valores[cont + j] = 1;
        }
        cont += tam;
    }
    A.linha_index[ncliente] = cont;

    return A;
}

// Multiplicacao C = A * A^T diretamente na estrutura CSR
CSRInt multiplicarCSRTransposta(const CSRInt& A) {
    int n = A.linhas;
    CSRInt C;
    C.linhas = n;
    C.colunas = n;
    C.linha_index = (int*)malloc((n + 1) * sizeof(int));

    // Capacidade dinamica inicial para armazenar apenas elementos com soma > 0
    int capacidade = (A.total_elementos > 0) ? (A.total_elementos * 4) : 1024;
    C.valores = (int*)malloc(capacidade * sizeof(int));
    C.col_index = (int*)malloc(capacidade * sizeof(int));

    int cont = 0;
    C.linha_index[0] = 0;

    for (int i = 0; i < n; i++) {
        int inicio_i = A.linha_index[i];
        int fim_i = A.linha_index[i + 1];

        for (int j = 0; j < n; j++) {
            int p1 = inicio_i;
            int p2 = A.linha_index[j];
            int fim_j = A.linha_index[j + 1];

            int soma = 0;

            // Produto interno entre a linha i e a linha j usando os ponteiros CSR
            while (p1 < fim_i && p2 < fim_j) {
                if (A.col_index[p1] < A.col_index[p2]) {
                    p1++;
                } else if (A.col_index[p1] > A.col_index[p2]) {
                    p2++;
                } else {
                    soma += A.valores[p1] * A.valores[p2];
                    p1++;
                    p2++;
                }
            }

            // Adiciona na estrutura CSR da matriz C apenas se houver intersecao (soma > 0)
            if (soma > 0) {
                if (cont >= capacidade) {
                    capacidade *= 2;
                    C.valores = (int*)realloc(C.valores, capacidade * sizeof(int));
                    C.col_index = (int*)realloc(C.col_index, capacidade * sizeof(int));
                }
                C.valores[cont] = soma;
                C.col_index[cont] = j;
                cont++;
            }
        }
        C.linha_index[i + 1] = cont;
    }

    C.total_elementos = cont;
    if (cont > 0) {
        C.valores = (int*)realloc(C.valores, cont * sizeof(int));
        C.col_index = (int*)realloc(C.col_index, cont * sizeof(int));
    }

    return C;
}

// Calcula a matriz de similaridade S em formato CSR a partir de I (em CSR) e A (em CSR)
CSRDouble calcularJaccardCSR(const CSRInt& I, const CSRInt& A) {
    CSRDouble S;
    S.linhas = I.linhas;
    S.colunas = I.colunas;
    S.total_elementos = I.total_elementos;

    S.valores = (double*)malloc(S.total_elementos * sizeof(double));
    S.col_index = (int*)malloc(S.total_elementos * sizeof(int));
    S.linha_index = (int*)malloc((S.linhas + 1) * sizeof(int));

    for (int i = 0; i < S.linhas; i++) {
        S.linha_index[i] = I.linha_index[i];
        int tamanho_Pi = A.linha_index[i + 1] - A.linha_index[i]; // |P_i|

        for (int k = I.linha_index[i]; k < I.linha_index[i + 1]; k++) {
            S.col_index[k] = I.col_index[k];
            if (tamanho_Pi > 0) {
                S.valores[k] = 1.0 - ((double)I.valores[k] / tamanho_Pi);
            } else {
                S.valores[k] = 1.0;
            }
        }
    }
    S.linha_index[S.linhas] = I.linha_index[I.linhas];

    return S;
}

CSRDouble calcularSimilaridadeCSR(const list<vector<int>>& lista_compra, int ncliente, int nproduto) {
    CSRInt A = construirCSRCompras(lista_compra, ncliente, nproduto);
    CSRInt I = multiplicarCSRTransposta(A);
    CSRDouble S = calcularJaccardCSR(I, A);

    liberarCSRInt(&A);
    liberarCSRInt(&I);

    return S;
}

int encontrarClienteMaisSimilarCSR(const CSRDouble& S, int cliente_alvo, double* menor_dist) {
    int mais_similar = -1;
    double menor = 2.0;

    for (int k = S.linha_index[cliente_alvo]; k < S.linha_index[cliente_alvo + 1]; k++) {
        int j = S.col_index[k];
        if (j != cliente_alvo && S.valores[k] < menor) {
            menor = S.valores[k];
            mais_similar = j;
        }
    }
    if (menor_dist != NULL) {
        *menor_dist = menor;
    }
    return mais_similar;
}

size_t calcularMemoriaCSRInt(const CSRInt& M) {
    return (M.total_elementos * sizeof(int)) + (M.total_elementos * sizeof(int)) + ((M.linhas + 1) * sizeof(int));
}

size_t calcularMemoriaCSRDouble(const CSRDouble& M) {
    return (M.total_elementos * sizeof(double)) + (M.total_elementos * sizeof(int)) + ((M.linhas + 1) * sizeof(int));
}

void liberarCSRInt(CSRInt* M) {
    free(M->valores);
    free(M->col_index);
    free(M->linha_index);
    M->valores = NULL;
    M->col_index = NULL;
    M->linha_index = NULL;
}

void liberarCSRDouble(CSRDouble* M) {
    free(M->valores);
    free(M->col_index);
    free(M->linha_index);
    M->valores = NULL;
    M->col_index = NULL;
    M->linha_index = NULL;
}

// Testador exigido nos Entregaveis da Atividade 5 (Desempenho + Economia de Memoria)
int similaridadeCSR(listaCompras &lista) {
    int nCliente = (int)lista.compras.size();
    int nProd = (int)lista.codigo_produtos.size();
    int cliente_alvo = (nCliente > 840) ? 840 : 0;

    // 1. Medicao da abordagem Densa
    clock_t inicio_densa = clock();
    double** S_densa = alocarMatrizDouble(nCliente, nCliente);
    calcularSimilaridade(lista.compras, S_densa, nCliente, nProd);
    clock_t fim_densa = clock();
    double tempo_densa = (double)(fim_densa - inicio_densa) / CLOCKS_PER_SEC;

    // 2. Medicao da abordagem Esparsa (CSR)
    clock_t inicio_csr = clock();
    CSRInt A_csr = construirCSRCompras(lista.compras, nCliente, nProd);
    CSRInt I_csr = multiplicarCSRTransposta(A_csr);
    CSRDouble S_csr = calcularJaccardCSR(I_csr, A_csr);
    clock_t fim_csr = clock();
    double tempo_csr = (double)(fim_csr - inicio_csr) / CLOCKS_PER_SEC;

    // Calculo de Memoria (Bytes -> KB)
    size_t mem_compra_densa = (size_t)nCliente * nProd * sizeof(int);
    size_t mem_sim_densa = (size_t)nCliente * nCliente * sizeof(double);
    size_t mem_compra_csr = calcularMemoriaCSRInt(A_csr);
    size_t mem_sim_csr = calcularMemoriaCSRDouble(S_csr);

    double menor_dist = 1.0;
    int mais_similar = encontrarClienteMaisSimilarCSR(S_csr, cliente_alvo, &menor_dist);

    printf("\n=== Resultado Etapa 5 (Matriz Esparsa CSR vs Densa) ===\n");
    if (mais_similar != -1) {
        printf("Cliente consultado: Indice %d (Codigo: %s)\n",
               cliente_alvo, lista.clientes[cliente_alvo].c_str());
        printf("Cliente mais similar (CSR): Indice %d (Codigo: %s)\n",
               mais_similar, lista.clientes[mais_similar].c_str());
        printf("Distancia de Jaccard s(i,j): %.4f\n", menor_dist);
    }

    printf("\n--- Comparativo de Desempenho e Memoria ---\n");
    printf("Tempo Matriz Densa:   %.6f s\n", tempo_densa);
    printf("Tempo Matriz CSR:     %.6f s\n", tempo_csr);
    printf("Memoria Compras Densa: %.2f KB | CSR (total_elementos=%d): %.2f KB\n",
           mem_compra_densa / 1024.0, A_csr.total_elementos, mem_compra_csr / 1024.0);
    printf("Memoria Similaridade Densa: %.2f KB | CSR (total_elementos=%d): %.2f KB\n",
           mem_sim_densa / 1024.0, S_csr.total_elementos, mem_sim_csr / 1024.0);

    liberarMatrizDouble(S_densa, nCliente);
    liberarCSRInt(&A_csr);
    liberarCSRInt(&I_csr);
    liberarCSRDouble(&S_csr);

    return 0;
}