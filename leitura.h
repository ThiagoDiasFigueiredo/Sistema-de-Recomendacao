#ifndef LEITURA_H
#define LEITURA_H
#define REGISTROS 1600
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
}listaCompras;


int leitura(int argc, char *argv[]);

#endif