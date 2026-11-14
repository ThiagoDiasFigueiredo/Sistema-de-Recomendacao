#include "leitura.h"

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <string>
using namespace std;

// Função para ordenar os structs pelo nome do produto
bool comparaPorNome(const Dados &a, const Dados &b) {
    return string(a.nome_produto) < string(b.nome_produto);
}
bool comparaPorCliente(const Dados &a, const Dados &b){
    return string(a.cliente) < string(b.cliente);
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
    Dados dados[1600];

    int total = 0;    
    int lixo = 0;
    int pos_cliente_unico = 0;
    int pos_repetido_nome = 0;
    
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
    //Filtrar os clientes
    
    sort(dados, dados + total, comparaPorCliente);
    for (int i = 0; i < total; i++) {
        if (i > 0 && string(dados[i].cliente) == string(dados[i - 1].cliente)) {
            continue;
        }else{
            lista.codigo_clientes.push_back(dados[i].cliente);
            pos_cliente_unico++; 
        }
    }
    
    // Filtrar o nome dos produtos
    sort(dados, dados + total, comparaPorNome);
    for (int i = 0; i < total; i++) {
        if (i > 0 && string(dados[i].nome_produto) == string(dados[i - 1].nome_produto)) {
            continue;
        }else{
            lista.nome_produtos.push_back(dados[i].nome_produto);
            pos_repetido_nome++;
        }
       
    }
    //Construindo o map dos codigos dos clientes
    int indice = 0;
    for(int i = 0; i < total; i++){
        string cod = dados[i].cliente;
        if(lista.codigo.find(cod) == lista.codigo.end()){ //Cliente não está no map
            lista.codigo[cod] = indice;
            lista.codigo_clientes.push_back(cod);
            indice++;
        }
    }
    //Construindo o map dos produtos
    int pos_repetido_produto = 0;

    for (int i = 0; i < total; i++) {
    int cod = dados[i].produto;

    if (lista.codigo_prod.find(cod) == lista.codigo_prod.end()) {   // produto ainda não está no map
        lista.codigo_prod[cod] = pos_repetido_produto;           // ID = próximo número livre
        lista.codigo_produtos.push_back(cod);
        pos_repetido_produto++;
        }
    }
    //Monta o vector compra
    // Monta o vector compras
    sort(dados, dados + total, comparaPorCliente);   // agrupa os clientes

    int pos_clientes = -1;                            // posição do cliente atual
    for (int i = 0; i < total; i++) {

        if (i == 0 || string(dados[i].cliente) != string(dados[i - 1].cliente)) {
            pos_clientes++;
            lista.clientes.push_back(dados[i].cliente);
            vector<int> vazia;
            lista.compras.push_back(vazia);
        }

        lista.compras[pos_clientes].push_back(dados[i].produto);
    }
    
    //Atividade 1.1
     /* for(int i = 0; i < pos_cliente_unico; i++){
        cout << "[" << i+1 << "] elemento: " << lista.codigo_clientes[i] << "\n";
    } */    
   
    //atividade 1.2
    /* for (int i = 0; i < pos_cliente_unico; i++){
        cout << lista.codigo_clientes[i]<< "->" << i << "\n";  
    } */  

    //Atividade 1.3
    
     /* for(int i = 0; i < pos_repetido_nome; i++){
        cout << "[" << i+1 << "] elemento: " << lista.nome_produtos[i] << "\n";
    } */   
 
    
    //Atividade 1.4
      /* for (auto &p : lista.codigo_prod) { //exibe em ordem crescente as chaves
    cout << p.first << " -> " << p.second << "\n";   // código -> ID
    }  */   

    //atividade 1.5
    
        for (int i = 0; i < pos_clientes; i++) {
            cout << lista.clientes[i] << " comprou:";
            int tam = lista.compras[i].size();
            for (int j = 0; j < tam; j++) {
                cout << " " << lista.compras[i][j];
            }
            cout << "\n";
        }  
    
    printf("\nTotal de registros validos lidos: %d\n", total);
    printf("Total de linhas invalidas: %d\n", lixo);
    printf("Total de produtos unicos: %d\n", pos_repetido_nome);
    printf("Total de clientes unicos: %d\n",pos_cliente_unico);

    return 0;

}

