#include "leitura.h"

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <string>
using namespace std;

bool comparaPorNome(const Dados &a, const Dados &b) {
    return string(a.nome_produto) < string(b.nome_produto);
}
bool comparaPorCliente(const Dados &a, const Dados &b){
    return string(a.cliente) < string(b.cliente);
}

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
    for (int i = 0; i < pos_cliente; i++) {
        cout << lista.clientes[i] << " comprou:";
        int tam = lista.compras[i].size();
        for (int j = 0; j < tam; j++) {
            cout << " " << lista.compras[i][j];
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
    Dados dados[1700];

    int total = 0;    
    int lixo = 0;
    int qtd_cliente_unico = 0;
    int qtd_repetido_nome = 0;
    
    // Descarta o cabeçalho inicial
    fscanf(file, "%*[^\n]");

    char linha_lixo[512];

    while (total < 1600) {
        int lido = fscanf(file, " %8[^,],%19[^,],%d,%199[^\r\n]",
            dados[total].data,
            dados[total].cliente,
            &dados[total].produto,
            dados[total].nome_produto);
        
        if (lido == 4) {
            total++; // Leu corretamente
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
    //Filtrar os clientes ATIVIDADE 1.1
    
    sort(dados, dados + total, comparaPorCliente);
    for (int i = 0; i < total; i++) {
        if (i > 0 && string(dados[i].cliente) == string(dados[i - 1].cliente)) {
            continue;
        }else{
            lista.codigo_clientes.push_back(dados[i].cliente);
            
            qtd_cliente_unico++; 
        }
        
    }
    //Construindo o map dos codigos dos clientes ATIVIDADE 1.2
    int indice = 0;
    for(int i = 0; i < total; i++){
        string cod = dados[i].cliente;
        if(lista.codigo.find(cod) == lista.codigo.end()){ //Cliente não está no map
            lista.codigo[cod] = indice;
            lista.codigo_clientes.push_back(cod);
            indice++;
        }
    }

    // Filtrar o nome dos produtos ATIVIDADE 1.3
    sort(dados, dados + total, comparaPorNome);
    for (int i = 0; i < total; i++) {
        if (i > 0 && string(dados[i].nome_produto) == string(dados[i - 1].nome_produto)) {
            continue;
        }else{
            lista.nome_produtos.push_back(dados[i].nome_produto);
            
            qtd_repetido_nome++;
        }
       
    }
    
    //Construindo o map dos produtos ATIVIDADE 1.4
    int qtd_repetido_produto = 0;

    for (int i = 0; i < total; i++) {
    int cod = dados[i].produto;

    if (lista.codigo_prod.find(cod) == lista.codigo_prod.end()) {   // produto ainda não está no map
        lista.codigo_prod[cod] = qtd_repetido_produto;           // ID = próximo número livre
        lista.codigo_produtos.push_back(cod);
        qtd_repetido_produto++;
        }
    }

    //Monta o vector compra ATIVIDADE 1.5
    sort(dados, dados + total, comparaPorCliente); 
    int pos_cliente = -1;
    for (int i = 0; i < total; i++) {

        
        if (i == 0 || string(dados[i].cliente) != string(dados[i - 1].cliente)) {
            pos_cliente++;
            lista.clientes.push_back(dados[i].cliente);
            vector<int> vazia;
            lista.compras.push_back(vazia);
        }

        
        int id_prod = lista.codigo_prod[dados[i].produto];
        bool repetido = false;
        for (int j = 0; j < (int)lista.compras[pos_cliente].size(); j++) {
            if (lista.compras[pos_cliente][j] == id_prod) {
                repetido = true;
            }
        }
        if (!repetido) {
            lista.compras[pos_cliente].push_back(id_prod);
        }
    } 
    
    //EXECUÇÃO DAS ATIVIDADES

    //AtividadeUm(lista, qtd_cliente_unico);
    //AtividadeDois(lista, qtd_cliente_unico);
    //AtividadeTres(lista, qtd_repetido_nome);
    //AtividadeQuatro(lista,qtd_repetido_produto);
    //AtividadeCinco(lista, pos_cliente);   
    
    printf("\nTotal de registros validos lidos: %d\n", total);
    printf("Total de linhas invalidas: %d\n", lixo);
    printf("Total de produtos unicos: %d\n", qtd_repetido_nome);
    printf("Total de clientes unicos: %d\n",qtd_cliente_unico);

    return 0;

}