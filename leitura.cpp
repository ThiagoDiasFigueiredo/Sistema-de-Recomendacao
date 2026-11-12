#include "leitura.h"

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

// Função para ordenar os structs pelo nome do produto
bool comparaPorNome(const Dados &a, const Dados &b) {
    return string(a.nome_produto) < string(b.nome_produto);
}
bool comparaPorCliente(const Dados &a, const Dados &b){
    return string(a.cod_cliente) < string(b.cod_cliente);
}

int leitura(int argc, char *argv[]) {
    
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
    listaCompras lista;

    int records = 0;    
    int lixo = 0;
    int qtd_repetido_cliente = 0;
    int qtd_repetido_nome = 0;
    
    // Descarta o cabeçalho inicial
    fscanf(file, "%*[^\n]");

    char linha_lixo[512];

    while (records < 1600) {
        int lido = fscanf(file, " %8[^,],%19[^,],%d,%199[^\r\n]",
            dados[records].data,
            dados[records].cod_cliente,
            &dados[records].cod_produto,
            dados[records].nome_produto);
        
        if (lido == 4) {
            records++; // Leu corretamente
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
    
    sort(dados, dados + records, comparaPorCliente);
    for (int i = 0; i < records; i++) {
        if (i > 0 && string(dados[i].cod_cliente) == string(dados[i - 1].cod_cliente)) {
            continue;
        }else{
            lista.codigo_clientes.push_back(dados[i].cod_cliente);
            qtd_repetido_cliente++; 
        }
        
    }

    // Filtrar o nome dos produtos
    sort(dados, dados + records, comparaPorNome);
    for (int i = 0; i < records; i++) {
        if (i > 0 && string(dados[i].nome_produto) == string(dados[i - 1].nome_produto)) {
            continue;
        }else{
            lista.nome_produtos.push_back(dados[i].nome_produto);
            
            qtd_repetido_nome++;
        }
       
    }
    //Construindo o map dos codigos dos clientes
    int indice = 0;
    for(int i = 0; i < records; i++){
        string cod = dados[i].cod_cliente;
        if(lista.codigo.find(cod) == lista.codigo.end()){ //Cliente não está no map
            lista.codigo[cod] = indice;
            lista.codigo_clientes.push_back(cod);
            indice++;
        }
    }
    int qtd_repetido_produto = 0;

    for (int i = 0; i < records; i++) {
    int cod = dados[i].cod_produto;

    if (lista.codigo_prod.find(cod) == lista.codigo_prod.end()) {   // produto ainda não está no map
        lista.codigo_prod[cod] = qtd_repetido_produto;           // ID = próximo número livre
        lista.codigo_produtos.push_back(cod);
        qtd_repetido_produto++;
    }
}

    // Exibe os produtos únicos filtrados
    /* for(int i = 0; i < qtd_repetido_cliente; i++){
        cout << "[" << i+1 << "] elemento: " << lista.codigo_clientes[i] << "\n";
    }
    for(int i = 0; i < qtd_repetido_nome; i++){
        cout << "[" << i+1 << "] elemento: " << lista.nome_produtos[i] << "\n";
    }
 */
    /* for (int i = 0; i < qtd_repetido_cliente; i++){
        cout << lista.codigo_clientes[i]<< "->" << i << "\n" << i;  
   } */ 
        
    for (auto &par : lista.codigo_prod) {
    cout << par.first << " -> " << par.second << "\n";   // código -> ID
    }
    printf("\nTotal de registros validos lidos: %d\n", records);
    printf("Total de linhas invalidas: %d\n", lixo);
    printf("Total de produtos unicos: %d\n", qtd_repetido_nome);
    printf("Total de clientes unicos: %d\n",qtd_repetido_cliente);
    
    return 0;
}
