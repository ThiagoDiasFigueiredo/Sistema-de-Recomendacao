#include <stdio.h>

int main(int argc, char *argv[]) {
    // Verifica se o nome do arquivo foi informado.
    if (argc != 2) {
        printf("Uso: %s <arquivo.csv>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    char data[9];
    char cliente[20];
    int produto;
    char nome[200];

    // Descarta o cabeçalho sem armazená-lo.
    fscanf(arquivo, "%*[^\n]");

    // O espaço inicial consome as quebras de linha.
    while (fscanf(arquivo, " %8[^,],%19[^,],%d,%199[^\r\n]",
                  data, cliente, &produto, nome) == 4) {
        printf("Data: %s | Cliente: %s | Produto: %d | Nome: %s\n",
               data, cliente, produto, nome);
    }

    fclose(arquivo);
    return 0;
}