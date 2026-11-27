#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>
#include <list>
#include <tuple>
#include <ctime>
#include <cstdlib>

#include "alocar_matriz.h"
#include "Similaridade.h"
#include "matrizes_esparsas.h"
#include "recomendacao.h"

namespace py = pybind11;
using namespace std;

// Retorna: (mais_similar, distancia_jaccard, tempo_segundos, mem_densa_kb, mem_csr_kb, econ_compra)
tuple<int, double, double, double, double, double> executar_similaridade(
    const vector<list<int>>& compras,
    int nCliente,
    int nProd,
    int cliente_alvo,
    int opcao
) {
    if (opcao == 1 || opcao == 2) {
        clock_t inicio = clock();
        double** S = alocarMatrizDouble(nCliente, nCliente);

        if (opcao == 1) {
            calcularSimilaridade(compras, S, nCliente, nProd);
        } else {
            calcularSimilaridadeOtimizada(compras, S, nCliente, nProd);
        }

        int mais_similar = encontrarClienteMaisSimilar(S, nCliente, cliente_alvo);
        double dist = (mais_similar != -1) ? S[cliente_alvo][mais_similar] : 1.0;

        liberarMatrizDouble(S, nCliente);
        clock_t fim = clock();

        double tempo = double(fim - inicio) / CLOCKS_PER_SEC;
        double mem_densa_kb = ((double)nCliente * (double)nCliente * sizeof(double)) / 1024.0;
        return make_tuple(mais_similar, dist, tempo, mem_densa_kb, 0.0, 0.0);
    } else {
        clock_t inicio_csr = clock();
        CSRInt A_csr = construirCSRCompras(compras, nCliente, nProd);
        CSRInt I_csr = multiplicarCSRTransposta(A_csr);
        CSRDouble S_csr = calcularJaccardCSR(I_csr, A_csr);
        clock_t fim_csr = clock();

        double tempo_csr = (double)(fim_csr - inicio_csr) / CLOCKS_PER_SEC;

        size_t mem_compra_densa = (size_t)nCliente * (size_t)nProd * sizeof(int);
        size_t mem_compra_csr = calcularMemoriaCSRInt(A_csr);
        double econ_compra = 100.0 - ((double(mem_compra_csr) * 100.0) / (double)mem_compra_densa);

        double menor_dist = 1.0;
        int mais_similar = encontrarClienteMaisSimilarCSR(S_csr, cliente_alvo, menor_dist);

        return make_tuple(
            mais_similar,
            menor_dist,
            tempo_csr,
            mem_compra_densa / 1024.0,
            mem_compra_csr / 1024.0,
            econ_compra
        );
    }
}

// Retorna: lista de tuplas (id_produto, nota_R)
vector<tuple<int, double>> executar_recomendacao(
    const vector<list<int>>& compras,
    int nCliente,
    int nProd,
    int cliente,
    int k
) {
    CSRDouble S = calcularSimilaridadeCSR(compras, nCliente, nProd);

    int qtdRec = 0;
    Ranking* rec = recomendarCSR(compras, S, nProd, cliente, k, &qtdRec);

    vector<tuple<int, double>> resultado;
    for (int i = 0; i < qtdRec; i++) {
        resultado.push_back(make_tuple(rec[i].id, rec[i].valor));
    }

    free(rec);
    return resultado;
}

PYBIND11_MODULE(solver, m) {
    m.doc() = "Integracao Python-C++ do Sistema de Recomendacao";
    m.def("similaridade", &executar_similaridade);
    m.def("recomendacao", &executar_recomendacao);
}