#include "matrizes_esparsas.h"
#include "Similaridade.h"
#include "alocar_matriz.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>

using namespace std;

// Converte a lista de compras para a Matriz de Compras A no formato CSR
CSRInt construirCSRCompras(const list<vector<int>>& lista_compra, int ncliente, int nproduto) {
    CSRInt A;
    A.linhas = ncliente;
    A.colunas = nproduto;

    //Conta o total de elementos comprados, ou seja, não nulos
    int total_elementos = 0;
    for (const vector<int>& produtos_cliente : lista_compra) {
        total_elementos += (int)produtos_cliente.size();
    }
    A.total_elementos = total_elementos;

    A.valores = (int*)malloc(total_elementos * sizeof(int));
    A.col_index = (int*)malloc(total_elementos * sizeof(int));
    A.linha_index = (int*)malloc((ncliente + 1) * sizeof(int));

    int cont = 0;
    int i = 0;

    // Preenche os vetores CSR linha por linha
    for (const vector<int>& produtos_cliente : lista_compra) {//pega a ficha de cada cliente 
        //para quando processar todos os diferentes clientes
        if (i >= ncliente) {
            break;
        }

        A.linha_index[i] = cont;
        int tam = (int)produtos_cliente.size();//quantos produtos o cliente comprou

        // Copia os produtos comprados pelo cliente i
        for (int j = 0; j < tam; j++) {
            A.col_index[cont + j] = produtos_cliente[j];//indica onde o cliente começa e vai contando a partir disso
        }

        // Ordena os indices de coluna da linha i para permitir a intersecao em CSR
        sort(A.col_index + cont, A.col_index + cont + tam);

        for (int j = 0; j < tam; j++) {
            A.valores[cont + j] = 1;
        }
        cont += tam;
        i++;
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
    int capacidade = A.total_elementos * 4;
    C.valores = (int*)malloc(capacidade * sizeof(int));
    C.col_index = (int*)malloc(capacidade * sizeof(int));

    int cont = 0;
    C.linha_index[0] = 0;

    //olha onde começam e onde terminam os produtos comprados pelo cliente que vamos comparar e pelo cliente comparado
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
    C.valores = (int*)realloc(C.valores, cont * sizeof(int));
    C.col_index = (int*)realloc(C.col_index, cont * sizeof(int));

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
        int tamanho_Pi = A.linha_index[i + 1] - A.linha_index[i]; // Onde termina - começo

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

    //percorre o S um por vez para descobrir qual o mais similar
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
    int cliente_alvo = 791;

    // Medicao da abordagem Esparsa (CSR)
    clock_t inicio_csr = clock();
    CSRInt A_csr = construirCSRCompras(lista.compras, nCliente, nProd);
    CSRInt I_csr = multiplicarCSRTransposta(A_csr);
    CSRDouble S_csr = calcularJaccardCSR(I_csr, A_csr);
    clock_t fim_csr = clock();
    double tempo_csr = (double)(fim_csr - inicio_csr) / CLOCKS_PER_SEC;

    // Calculo de economia de memoria (CSR vs Densa)
    size_t mem_compra_densa = (size_t)nCliente * (size_t)nProd * sizeof(int);
    size_t mem_sim_densa = (size_t)nCliente * (size_t)nCliente * sizeof(double);
    size_t mem_compra_csr = calcularMemoriaCSRInt(A_csr);
    size_t mem_sim_csr = calcularMemoriaCSRDouble(S_csr);

    double econ_compra = (1.0 - ((double)mem_compra_csr / (double)mem_compra_densa)) * 100.0;
    double econ_sim = (1.0 - ((double)mem_sim_csr / (double)mem_sim_densa)) * 100.0;

    double menor_dist = 1.0;
    int mais_similar = encontrarClienteMaisSimilarCSR(S_csr, cliente_alvo, &menor_dist);

    printf("\n=== Resultado Similaridade (Matriz Esparsa CSR) ===\n");
    if (mais_similar != -1) {
        printf("Cliente consultado: Indice %d (Codigo: %s)\n",
               cliente_alvo, lista.clientes[cliente_alvo].c_str());
        printf("Cliente mais similar: Indice %d (Codigo: %s)\n",
               mais_similar, lista.clientes[mais_similar].c_str());
        printf("Distancia de Jaccard s(i,j): %.4f\n", menor_dist);
    }
    printf("Tempo CSR: %.6f s\n", tempo_csr);
    printf("Memoria Compras:      Densa = %.2f KB | CSR = %.2f KB | Economia = %.2f%%\n",
           mem_compra_densa / 1024.0, mem_compra_csr / 1024.0, econ_compra);
    printf("Memoria Similaridade: Densa = %.2f KB | CSR = %.2f KB | Economia = %.2f%%\n",
           mem_sim_densa / 1024.0, mem_sim_csr / 1024.0, econ_sim);

    liberarCSRInt(&A_csr);
    liberarCSRInt(&I_csr);
    liberarCSRDouble(&S_csr);

    return 0;
}