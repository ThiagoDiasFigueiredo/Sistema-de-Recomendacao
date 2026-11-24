#include "recomendacao.h"
#include "matrizes_esparsas.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

bool comparaPorValor(const Ranking &a, const Ranking &b) {
    if (a.valor != b.valor) {
        return a.valor < b.valor;
    }
    return a.id < b.id;
}

// Atividade 3.1 (adaptada para CSR) - Percorre apenas a linha c da matriz esparsa S
int identificarVizinhosCSR(const CSRDouble& S, int c, int* L, double* distL) {
    //seleciona os clientes que tem pelo menos um produto em comum
    int tamL = 0;
    for (int k = S.linha_index[c]; k < S.linha_index[c + 1]; k++) {
        int vizinho = S.col_index[k];
        double dist = S.valores[k];
        if (vizinho != c && dist < 1.0) {
            L[tamL] = vizinho;
            distL[tamL] = dist;
            tamL++;
        }
    }
    return tamL;
}

// Atividade 3.2 - Inicializa os vetores de ranqueamento
void inicializaRanqueamento(const vector<list<int>> &compras, int numProd, int c,
                            double* vetorR, int* marca, int* comprado) {
    for (int i = 0; i < numProd; i++) {
        vetorR[i] = 1.0;   // Cada produto recebe 1
        marca[i] = -1;     // Produto nao atualizado
        comprado[i] = 0;   // Cliente ainda nao comprou
    }

    for (auto it = compras[c].begin(); it != compras[c].end(); ++it) {
        int prodId = *it;
        if (prodId >= 0 && prodId < numProd) {
            comprado[prodId] = 1;
        }
    }
}

// Atividade 3.3 (adaptada para CSR) - Usa diretamente as distancias extraidas da CSR
void calcularRanqueamentoCSR(double* notaProduto, int* ultimoVizinho, const int* clienteComprou,
                             const vector<list<int>> &compras, const int* vizinhos,
                             const double* distVizinhos, int qtdVizinhos, int qtdProdutos) {
    for (int posVizinho = 0; posVizinho < qtdVizinhos; posVizinho++) {
        int vizinho = vizinhos[posVizinho];
        double dist = distVizinhos[posVizinho];

        for (auto it = compras[vizinho].begin(); it != compras[vizinho].end(); ++it) {
            int produto = *it;
            if (produto >= 0 && produto < qtdProdutos) {
                if (clienteComprou[produto] == 0 && ultimoVizinho[produto] != vizinho) {
                    notaProduto[produto] = notaProduto[produto] * dist;
                    ultimoVizinho[produto] = vizinho;
                }
            }
        }
    }
}

// Atividade 3.4 - Ordena candidatos e seleciona os k melhores
int ordenaEseleciona(const double* vetorR, const int* marca, const int* comprado,
                     int nProd, int k, Ranking* resultado) {
    Ranking* candidatos = (Ranking*)malloc(nProd * sizeof(Ranking));
    int qtdCandidatos = 0;

    for (int p = 0; p < nProd; p++) {
        if (comprado[p] == 0 && marca[p] != -1) {
            candidatos[qtdCandidatos].id = p;
            candidatos[qtdCandidatos].valor = vetorR[p];
            qtdCandidatos++;
        }
    }

    sort(candidatos, candidatos + qtdCandidatos, comparaPorValor);

    int qtdResultado = 0;
    for (int i = 0; i < qtdCandidatos && i < k; i++) {
        resultado[qtdResultado] = candidatos[i];
        qtdResultado++;
    }

    free(candidatos);
    return qtdResultado;
}

Ranking* recomendarCSR(const vector<list<int>> &compras, const CSRDouble& S,
                       int nProd, int c, int k, int* qtdRecomendados) {
    int capVizinhos = S.linha_index[c + 1] - S.linha_index[c];
    if (capVizinhos <= 0) {
        capVizinhos = 1;
    }

    int* L = (int*)malloc(capVizinhos * sizeof(int));
    double* distL = (double*)malloc(capVizinhos * sizeof(double));
    double* vetorR = (double*)malloc(nProd * sizeof(double));
    int* marca = (int*)malloc(nProd * sizeof(int));
    int* comprado = (int*)malloc(nProd * sizeof(int));
    Ranking* resultado = (Ranking*)malloc(k * sizeof(Ranking));

    int tamL = identificarVizinhosCSR(S, c, L, distL);
    inicializaRanqueamento(compras, nProd, c, vetorR, marca, comprado);
    calcularRanqueamentoCSR(vetorR, marca, comprado, compras, L, distL, tamL, nProd);
    *qtdRecomendados = ordenaEseleciona(vetorR, marca, comprado, nProd, k, resultado);

    free(L);
    free(distL);
    free(vetorR);
    free(marca);
    free(comprado);

    return resultado;
}

int recomendacao(listaCompras &lista, int cliente, int k) {
    int nCliente = (int)lista.compras.size();
    int nProd = (int)lista.codigo_produtos.size();

    if (cliente < 0 || cliente >= nCliente) {
        printf("Cliente invalido: %d\n", cliente);
        return 1;
    }

    // Usa a nova representacao de matriz esparsa CSR (Atividade 5)
    CSRDouble S = calcularSimilaridadeCSR(lista.compras, nCliente, nProd);

    int qtdRec = 0;
    Ranking* rec = recomendarCSR(lista.compras, S, nProd, cliente, k, &qtdRec);

    printf("\n=== Recomendacoes (via Matriz Esparsa CSR) ===\n");
    printf("Cliente consultado: Indice %d (Codigo: %s)\n",
           cliente, lista.clientes[cliente].c_str());

    if (qtdRec == 0) {
        printf("Nenhum produto para recomendar.\n");
    }
    for (int i = 0; i < qtdRec; i++) {
        printf("%d) Nome do produto: %s (indice %d) - R = %.4f\n",
               i + 1, lista.nome_produtos[rec[i].id].c_str(), rec[i].id, rec[i].valor);
    }

    free(rec);
    liberarCSRDouble(&S);
    return 0;
}