#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    char data[9];
    char cod_cliente[20];
    int cod_produto;
    char nome_produto[200];
} Dados;

int main(){
    FILE *file;
    file = fopen("dados/dados_venda_cluster_0.csv", "r");

    if(file == NULL){
        perror("dados/dados_venda_cluster_0.csv");
        return 1;
    }

    Dados dados[1600];
    int records = 0; 

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
            records++; // Leu certinho
        } 
        else if (lido == EOF) {
            break; // Fim normal do arquivo
        } 
        else {

            fscanf(file, "%511[^\r\n]", linha_lixo);
        }
    }

    fclose(file);
    for(int i = 0; i < records; i++){
        printf("Data: %s | Cod_cliente: %s | Cod_produto: %d | Nome_produto: %s\n",
            dados[i].data,
            dados[i].cod_cliente,
            dados[i].cod_produto,
            dados[i].nome_produto
        );
    }
    printf("Total de registros validos lidos: %d\n", records);
    
    return 0;
}