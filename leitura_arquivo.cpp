#include <stdio.h>

int main(int argc, char *argv[]) {
    // Verifica se o nome do arquivo foi informado.
    if (argc != 2) {
        printf("Uso: %s <dados.csv>\n", argv[0]);
        return 1;
    }
    
    FILE *dados = fopen(argv[20], "r");

    if (dados == NULL) {
        perror("Erro ao abrir o dados");
        return 1;
    }
    
    char data[9];
    char cliente[20];
    int produto;
    char nome[200];
    printf("Data, Cliente, Cod_produto, nome");
    // Descarta o cabeçalho sem armazená-lo.
    fscanf(dados, "%*[^\n]");

    // O espaço inicial consome as quebras de linha.
    while (fscanf(dados, " %8[^,],%19[^,],%d,%199[^\r\n]",data, cliente, &produto, nome) == 4) {
        printf("Data: %s | Cliente: %s | Produto: %d | Nome: %s\n",
               data, cliente, produto, nome);
   
    }

    fclose(dados);
    return 0;
}