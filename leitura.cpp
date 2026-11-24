#include "leitura.h"

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <string>
using namespace std;

void AtividadeUm(const listaCompras &lista, int qtd_cliente_unico) {
    for (int i = 0; i < qtd_cliente_unico; i++) {
        cout << "[" << i << "] elemento: " << lista.codigo_clientes[i] << "\n";
    }
}

void AtividadeDois(const listaCompras &lista, int qtd_cliente_unico) {

    for (int i = 0; i < qtd_cliente_unico; i++) {
        cout << lista.codigo_clientes[i] << "->" << i << "\n";
    }
}

void AtividadeTres(const listaCompras &lista, int qtd_repetido_nome) {
    for (int i = 0; i < qtd_repetido_nome; i++) {
        cout << "[" << i << "] elemento: " << lista.nome_produtos[i] << "\n";
    }
}

void AtividadeQuatro(const listaCompras &lista,int qnt_produto) {
    for(int i = 0; i < qnt_produto; i++){
        cout << lista.codigo_produtos[i] << "->" << i << "\n";
    }
}

void AtividadeCinco(const listaCompras &lista, int pos_cliente) {
    for (int i = 0; i < pos_cliente && i < (int)lista.compras.size(); i++) {
        cout << lista.clientes[i] << " comprou:";
        for (auto it = lista.compras[i].begin(); it != lista.compras[i].end(); ++it) {
            cout << " " << *it;
        }
        cout << "\n";
    }
}

int leitura(int argc, char *argv[], listaCompras &lista) {
    
    if (argc != 2) {
        printf("Uso: %s <dados/dados_venda_cluster_0.csv>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Erro ao abrir o file");
        return 1;
    }
// Variáveis simples apenas para ler a linha atual (sem vetor intermediário)
    char data[9];
    char cliente[20];
    int produto;
    char nome_produto[200];

    int total = 0;    
    int lixo = 0;
    int qtd_cliente_unico = 0;
    int qtd_repetido_nome = 0;
    int qtd_repetido_produto = 0;
    
    // Descarta o cabeçalho inicial
    fscanf(file, "%*[^\n]");

    char linha_lixo[512];

    while (total < 1600) {
        int lido = fscanf(file, " %8[^,],%19[^,],%d,%199[^\r\n]",
            data,
            cliente,
            &produto,
            nome_produto);
        
        if (lido == 4) {
            total++; // Leu corretamente

            string cod_cliente = cliente;
            string nome_prod = nome_produto;

            // Atividades 1.1 e 1.2: Controla clientes únicos usando find no map
            if (lista.codigo.find(cod_cliente) == lista.codigo.end()) {
                lista.codigo[cod_cliente] = qtd_cliente_unico;
                lista.codigo_clientes.push_back(cod_cliente);
                lista.clientes.push_back(cod_cliente);
                lista.compras.push_back(list<int>());
                qtd_cliente_unico++;
            }

            // Atividades 1.3 e 1.4: Controla produtos únicos usando find no map
            if (lista.codigo_prod.find(produto) == lista.codigo_prod.end()) {
                lista.codigo_prod[produto] = qtd_repetido_produto;
                lista.codigo_produtos.push_back(produto);
                lista.nome_produtos.push_back(nome_prod);
                qtd_repetido_produto++;
                qtd_repetido_nome++;
            }

            // Atividade 1.5: Usa o índice salvo no map para acessar a lista de compras do cliente
            int pos_cliente = lista.codigo[cod_cliente];
            int id_prod = lista.codigo_prod[produto];

            // Usa find para verificar se o cliente já comprou este produto
            if (find(lista.compras[pos_cliente].begin(), lista.compras[pos_cliente].end(), id_prod) == lista.compras[pos_cliente].end()) {
                lista.compras[pos_cliente].push_back(id_prod);
            }
        } 
        else if (lido == EOF) {
            break; // Fim normal do arquivo
        } 
        else {
            fscanf(file, "%511[^\r\n]", linha_lixo); // descarta linhas erradas
            lixo++;
        }
    }

    fclose(file);
    
    //EXECUÇÃO DAS ATIVIDADES

    //AtividadeUm(lista, qtd_cliente_unico); 
    //AtividadeDois(lista, qtd_cliente_unico);
    //AtividadeTres(lista, qtd_repetido_nome);
    //AtividadeQuatro(lista, qtd_repetido_produto);
    //AtividadeCinco(lista, qtd_cliente_unico);   
    
    printf("\nTotal de registros validos lidos: %d\n", total);
    printf("Total de linhas invalidas: %d\n", lixo);
    printf("Total de produtos unicos: %d\n", qtd_repetido_nome);
    printf("Total de clientes unicos: %d\n",qtd_cliente_unico);

    return 0;

}