#ifndef LEITURA_H
#define LEITURA_H

#include <iostream>
#include <vector>
#include<map>
using namespace std;

typedef struct 
{
    char data[9];
    char cod_cliente[20];
    int cod_produto;
    char nome_produto[200];

}Dados;

typedef struct 
{
    vector<string> codigo_clientes;
    vector<string> nome_produtos;
    map<string,int> codigo;
    vector<int> codigo_produtos;
    map<int,int> codigo_prod;

}listaCompras;


int leitura(int argc, char *argv[]);

#endif