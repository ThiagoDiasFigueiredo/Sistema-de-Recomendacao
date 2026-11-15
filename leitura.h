#ifndef LEITURA_H
#define LEITURA_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef struct 
{
    char data[9];
    char cliente[20];
    int produto;
    char nome_produto[200];

}Dados;

typedef struct
{
    vector<string> codigo_clientes;
    vector<string> nome_produtos;
    vector<int> codigo_produtos;
    map<int,int> codigo_prod;
    map<string,int> codigo;
    vector<string> clientes;          // código de cada cliente
    vector<vector<int>> compras;
    
}listaCompras;


int leitura(int argc, char *argv[], listaCompras &lista);
bool comparaPorNome(const Dados &a, const Dados &b);
bool comparaPorCliente(const Dados &a, const Dados &b);

// Atividades (chamadas de dentro da leitura)
void AtividadeUm(const listaCompras &lista, int qtd_cliente_unico);     // 1.1 clientes unicos
void AtividadeDois(const listaCompras &lista, int qtd_cliente_unico);   // 1.2 cliente -> indice
void AtividadeTres(const listaCompras &lista, int qtd_repetido_nome);   // 1.3 nomes dos produtos
void AtividadeQuatro(const listaCompras &lista);                        // 1.4 codigo do produto -> ID
void AtividadeCinco(const listaCompras &lista, int qtd_clientes);       // 1.5 produtos por cliente

#endif
