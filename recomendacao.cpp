#include "recomendacao.h"
#include "Similaridade.h"

#include <stdio.h>
#include <vector>
#include <algorithm>

bool comparaPorValor(const Ranking &a, const Ranking &b){
    if(a.valor != b.valor){
        return a.valor < b.valor;
    }
    return a.id < b.id;
}
//Atividade 3.1
void indentificarVizinhos(double** S,int numCliente,int c,vector<int> &L){ 
    for(int i = 0; i < numCliente;i++){
        if(i != c && S[c][i] < 1){ //Clientes diferentes e menores que 1
            L.push_back(i);
        }
    }
}

//Atividade 3.2
void inicializaRanqueamento(const vector<vector<int>> &compras,int numProd,int c,Ranqueamento &R){
    for(int i = 0; i < numProd; i++){
        R.vetorR.push_back(1); //Cada produto recebe 1
        R.marca.push_back(-1); //Produto não atualizado
        R.comprado.push_back(0);
    }
    for(int i = 0; i < (int)compras[c].size();i++){
        R.comprado[compras[c][i]] = 1;
    }
}

//Atividade 3.3
 void calcularRanqueamento(Ranqueamento &R, const vector<vector<int> >& compras,
                                 double** S, const vector<int> &L, int c) {
    for (int i = 0; i < (int)L.size(); i++) {
        int v = L[i];
        for (int j = 0; j < (int)compras[v].size(); j++) {
            int p = compras[v][j];
            if (R.comprado[p] == 0 && R.marca[p] != v) {
                R.vetorR[p] = R.vetorR[p] * S[c][v]; //QUANTO MAIS PARECIDO OS CLIENTES, MENOR A NOTA FICA
                R.marca[p] = v;
            }
        }
    }
}

//Atividade 3.4
void ordenaEseleciona(const Ranqueamento &R, int nProd, int k,vector<Ranking> &resultado) {
    vector<Ranking> candidatos;
    for (int p = 0; p < nProd; p++) {
        if (R.comprado[p] == 0 && R.marca[p] != -1) { //cliente C não comprou(==0) e q algum vizinho passou (!=-1)
            Ranking r;
            r.id = p;
            r.valor = R.vetorR[p];
            candidatos.push_back(r);
        }
    }

    sort(candidatos.begin(), candidatos.end(), comparaPorValor);

    for (int i = 0; i < (int)candidatos.size() && i < k; i++) {
        resultado.push_back(candidatos[i]);
    }
}

vector<Ranking> recomendar(const vector<vector<int> >& compras, double** S,int nCliente, int nProd, int c, int k) {
    vector<int> L;
    Ranqueamento R;
    vector<Ranking> resultado;

    indentificarVizinhos(S,nCliente,c,L);
    inicializaRanqueamento(compras, nProd, c, R);
    calcularRanqueamento(R, compras, S, L, c);
    ordenaEseleciona(R, nProd, k, resultado);
    return resultado;
}

int recomendacao(listaCompras &lista, int cliente, int k) {
    int nCliente = lista.compras.size();
    int nProd = lista.codigo_produtos.size();

    if (cliente < 0 || cliente >= nCliente) {
        printf("Cliente invalido: %d\n", cliente);
        return 1;
    }

    // Reaproveita as funções da etapa 2 para calcular S
    double** S = alocarMatrizDouble(nCliente, nCliente);
    calcularSimilaridade(lista.compras, S, nCliente, nProd);

    vector<Ranking> rec = recomendar(lista.compras, S, nCliente, nProd, cliente, k);

    printf("Cliente consultado: Indice %d (Codigo: %s)\n",
           cliente, lista.clientes[cliente].c_str());

    if (rec.empty()) {
        printf("Nenhum produto para recomendar.\n");
    }
    for (int i = 0; i < (int)rec.size(); i++) {
        printf("%d) Produto codigo %d (indice %d) - R = %.4f\n",
               i + 1, lista.codigo_produtos[rec[i].id], rec[i].id, rec[i].valor);
    }

    liberarMatrizDouble(S, nCliente);
    return 0;
}