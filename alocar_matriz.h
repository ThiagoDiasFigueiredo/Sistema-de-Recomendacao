#ifndef MATRIZ_H
#define MATRIZ_H

int** alocarMatrizInt(int linhas, int colunas);
double** alocarMatrizDouble(int linhas, int colunas);
void liberarMatrizInt(int** matriz, int linhas);
void liberarMatrizDouble(double** matriz, int linhas);

#endif