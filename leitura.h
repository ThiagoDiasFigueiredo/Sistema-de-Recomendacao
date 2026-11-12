#ifndef LEITURA_H
#define LEITURA_H

#include <iostream>
using namespace std;

typedef struct 
{
    char data[9];
    char cod_cliente[20];
    int cod_produto;
    char nome_produto[200];

}Dados;


int leitura(int argc, char *argv[]);

#endif