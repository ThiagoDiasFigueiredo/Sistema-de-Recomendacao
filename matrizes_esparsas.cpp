#include "matrizes_esparsas.h"

#include <cstdio>
#include <ctime>

using namespace std;

// Converte a lista de compras para a Matriz de Compras A no formato CSR
CSRInt construirCSRCompras(const vector<list<int>>& lista_compra, int ncliente, int nproduto) {
    CSRInt A;
    A.linhas = ncliente;
    A.colunas = nproduto;
    A.linha_index.resize(ncliente + 1, 0);

    int cont = 0;
    int i = 0;

    // Preenche os vetores CSR linha por linha (sem precisar ordenar)
    for (const list<int>& produtos_cliente : lista_compra) {
        if (i >= ncliente) {
            break;
        }

        A.linha_index[i] = cont;
        int tam = (int)produtos_cliente.size();

        for (int prod : produtos_cliente) {
            A.col_index.push_back(prod);
            A.valores.push_back(1);
        }

        cont += tam;
        i++;
    }
    A.linha_index[ncliente] = cont;
    A.total_elementos = cont;

    return A;
}

// Multiplicacao C = A * A^T usando vetor de marcacao (dispensa sort e ponteiros)
CSRInt multiplicarCSRTransposta(const CSRInt& A) {
    int n = A.linhas;
    CSRInt C;
    C.linhas = n;
    C.colunas = n;
    C.linha_index.resize(n + 1, 0);

    // Vetor auxiliar para marcar os produtos comprados pelo cliente i
    vector<int> marcado(A.colunas, 0);
    int cont = 0;

    for (int i = 0; i < n; i++) {
        int inicio_i = A.linha_index[i];
        int fim_i = A.linha_index[i + 1];

        // 1. Marca os produtos do cliente i
        for (int k = inicio_i; k < fim_i; k++) {
            int prod = A.col_index[k];
            marcado[prod] = A.valores[k]; // Guarda 1
        }

        // 2. Compara com todos os outros clientes j
        for (int j = 0; j < n; j++) {
            int inicio_j = A.linha_index[j];
            int fim_j = A.linha_index[j + 1];
            int soma = 0;

            for (int k = inicio_j; k < fim_j; k++) {
                int prod = A.col_index[k];
                if (marcado[prod] > 0) {
                    soma += marcado[prod] * A.valores[k]; //Se os clientes compraram, soma
                }
            }

            // Adiciona na estrutura CSR da matriz C apenas se houver intersecao
            if (soma > 0) {
                C.valores.push_back(soma);
                C.col_index.push_back(j);
                cont++;
            }
        }

        // 3. Limpa as marcacoes do cliente i para a proxima iteracao
        for (int k = inicio_i; k < fim_i; k++) {
            int prod = A.col_index[k];
            marcado[prod] = 0;
        }

        C.linha_index[i + 1] = cont;
    }

    C.total_elementos = cont;
    return C;
}

// Calcula a matriz de similaridade S em formato CSR a partir de I e A
CSRDouble calcularJaccardCSR(const CSRInt& I, const CSRInt& A) {
    CSRDouble S;
    S.linhas = I.linhas;
    S.colunas = I.colunas;
    S.total_elementos = I.total_elementos;

    S.valores.resize(S.total_elementos);
    S.col_index.resize(S.total_elementos);
    S.linha_index.resize(S.linhas + 1);

    for (int i = 0; i < S.linhas; i++) {
        S.linha_index[i] = I.linha_index[i];
        int tamanho_Pi = A.linha_index[i + 1] - A.linha_index[i];

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

CSRDouble calcularSimilaridadeCSR(const vector<list<int>>& lista_compra, int ncliente, int nproduto) {
    CSRInt A = construirCSRCompras(lista_compra, ncliente, nproduto);
    CSRInt I = multiplicarCSRTransposta(A);
    CSRDouble S = calcularJaccardCSR(I, A);

    // Nao precisa de funcoes liberarCSR: o std::vector libera a memoria sozinho ao sair da funcao
    return S;
}

int encontrarClienteMaisSimilarCSR(const CSRDouble& S, int cliente_alvo, double& menor_dist) {
    int mais_similar = -1;
    double menor = 2.0;

    for (int k = S.linha_index[cliente_alvo]; k < S.linha_index[cliente_alvo + 1]; k++) {
        int j = S.col_index[k];
        if (j != cliente_alvo && S.valores[k] < menor) {
            menor = S.valores[k];
            mais_similar = j;
        }
    }
    menor_dist = menor;
    return mais_similar;
}

size_t calcularMemoriaCSRInt(const CSRInt& M) {
    return (M.valores.size() * sizeof(int)) + (M.col_index.size() * sizeof(int)) + (M.linha_index.size() * sizeof(int));
}

size_t calcularMemoriaCSRDouble(const CSRDouble& M) {
    return (M.valores.size() * sizeof(double)) + (M.col_index.size() * sizeof(int)) + (M.linha_index.size() * sizeof(int));
}

// Testador exigido nos Entregaveis da Atividade 5
int similaridadeCSR(listaCompras &lista) {
    int nCliente = (int)lista.compras.size();
    int nProd = (int)lista.codigo_produtos.size();
    int cliente_alvo = 791;

    clock_t inicio_csr = clock();
    CSRInt A_csr = construirCSRCompras(lista.compras, nCliente, nProd);
    CSRInt I_csr = multiplicarCSRTransposta(A_csr);
    CSRDouble S_csr = calcularJaccardCSR(I_csr, A_csr);
    clock_t fim_csr = clock();
    double tempo_csr = (double)(fim_csr - inicio_csr) / CLOCKS_PER_SEC;

    size_t mem_compra_densa = (size_t)nCliente * (size_t)nProd * sizeof(int);
    size_t mem_sim_densa = (size_t)nCliente * (size_t)nCliente * sizeof(double);
    size_t mem_compra_csr = calcularMemoriaCSRInt(A_csr);
    size_t mem_sim_csr = calcularMemoriaCSRDouble(S_csr);

    double econ_compra = 100 - ((double(mem_compra_csr) * 100) /(double)mem_compra_densa);
    double econ_sim = 100 - (((double)mem_sim_csr) * 100 / (double)mem_sim_densa);

    double menor_dist = 1.0;
    int mais_similar = encontrarClienteMaisSimilarCSR(S_csr, cliente_alvo, menor_dist);

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

    return 0;
}