#ifndef LEITURA_H
#define LEITURA_H

#include <iostream>
#include <vector>
#include<map>


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


int leitura(int argc, char *argv[]);
bool comparaPorNome(const Dados &a, const Dados &b);
bool comparaPorCliente(const Dados &a, const Dados &b);

#endif