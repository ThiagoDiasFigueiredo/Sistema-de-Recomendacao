#include "recomendacao.h"
#include "Similaridade.h"
#include "alocar_matriz.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

bool comparaPorValor(const Ranking &a, const Ranking &b) {
    if (a.valor != b.valor) {
        return a.valor < b.valor;
    }
    return a.id < b.id;
}

// Atividade 3.1 - Identifica vizinhos usando vetor alocado com malloc
int indentificarVizinhos(double** S, int numCliente, int c, int* L) {
    int tamL = 0;
    for (int i = 0; i < numCliente; i++) {
        if (i != c && S[c][i] < 1.0) { // Clientes diferentes e distancia menor que 1
            L[tamL] = i;
            tamL++;
        }
    }
    return tamL;
}

// Atividade 3.2 - Inicializa os vetores de ranqueamento alocados com malloc
void inicializaRanqueamento(const vector<vector<int>> &compras, int numProd, int c,
                            double* vetorR, int* marca, int* comprado) {
    // Inicializa de 0 ate numProd (numProd + 1 posicoes por seguranca caso ID comece em 1)
    for (int i = 0; i < numProd; i++) {
        vetorR[i] = 1.0;   // Cada produto recebe 1
        marca[i] = -1;     // Produto nao atualizado
        comprado[i] = 0;   // Cliente ainda nao comprou
    }

    int qtdComprada = (int)compras[c].size();
    const int* produtosCliente = compras[c].data();
    for (int i = 0; i < qtdComprada; i++) {
        int prodId = produtosCliente[i];
        if (prodId >= 0 && prodId <= numProd) {
            comprado[prodId] = 1; // Marca 1 nos produtos ja comprados pelo cliente c
        }
    }
}

// Atividade 3.3 - Calcula o ranqueamento percorrendo ponteiros alocados com malloc
void calcularRanqueamento(double* vetorR, int* marca, const int* comprado,
                          const vector<vector<int>> &compras, double** S,
                          const int* L, int tamL, int c, int numProd) {
    for (int i = 0; i < tamL; i++) {
        int v = L[i];
        int qtdComprasVizinho = (int)compras[v].size();
        const int* produtosVizinho = compras[v].data();

        for (int j = 0; j < qtdComprasVizinho; j++) {
            int p = produtosVizinho[j];
            if (p >= 0 && p < numProd) {
                if (comprado[p] == 0 && marca[p] != v) {
                    vetorR[p] = vetorR[p] * S[c][v];
                    marca[p] = v;
                }
            }
        }
    }
}

// Atividade 3.4 - Ordena candidatos e seleciona os k melhores usando malloc
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

    std::sort(candidatos, candidatos + qtdCandidatos, comparaPorValor);

    int qtdResultado = 0;
    for (int i = 0; i < qtdCandidatos && i < k; i++) {
        resultado[qtdResultado] = candidatos[i];
        qtdResultado++;
    }

    free(candidatos);
    return qtdResultado;
}

Ranking* recomendar(const vector<vector<int>> &compras, double** S,
                    int nCliente, int nProd, int c, int k, int* qtdRecomendados) {
    // Aloca todas as estruturas dinamicamente com malloc
    int* L = (int*)malloc(nCliente * sizeof(int));
    double* vetorR = (double*)malloc((nProd + 1) * sizeof(double));
    int* marca = (int*)malloc((nProd + 1) * sizeof(int));
    int* comprado = (int*)malloc((nProd + 1) * sizeof(int));
    Ranking* resultado = (Ranking*)malloc(k * sizeof(Ranking));

    int tamL = indentificarVizinhos(S, nCliente, c, L);
    inicializaRanqueamento(compras, nProd, c, vetorR, marca, comprado);
    calcularRanqueamento(vetorR, marca, comprado, compras, S, L, tamL, c, nProd);
    *qtdRecomendados = ordenaEseleciona(vetorR, marca, comprado, nProd, k, resultado);

    // Libera os vetores auxiliares
    free(L);
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

    // Reaproveita as funcoes da etapa 2 e o alocar_matriz.h para calcular S
    double** S = alocarMatrizDouble(nCliente, nCliente);
    calcularSimilaridade(lista.compras, S, nCliente, nProd);

    int qtdRec = 0;
    Ranking* rec = recomendar(lista.compras, S, nCliente, nProd, cliente, k, &qtdRec);

    printf("Cliente consultado: Indice %d (Codigo: %s)\n",
           cliente, lista.clientes[cliente].c_str());

    if (qtdRec == 0) {
        printf("Nenhum produto para recomendar.\n");
    }
    for (int i = 0; i < qtdRec; i++) {
        printf("%d) Produto codigo %d (indice %d) - R = %.4f\n",
               i + 1, lista.codigo_produtos[rec[i].id], rec[i].id, rec[i].valor);
    }

    free(rec);
    liberarMatrizDouble(S, nCliente);
    return 0;
}