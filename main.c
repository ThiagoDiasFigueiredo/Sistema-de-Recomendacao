#include <stdio.h>
typedef struct 
{
    char data[9];
    char cod_cliente[20];
    int cod_produto;
    char nome_produto[200];
}Dados;

int main(){
    FILE *file;
    file = fopen("dados_venda_cluster_4.csv","r");

    if(file == NULL){
        printf("Algo deu errado!\n");
        return 1;
    }

    Dados dados[100000];

    int lido = 0;
    int records = 0; //quantos registros lemos do arquivo
    fscanf(file, "%*[^\n]");

    while(!feof(file)){ //continuar enquanto não atingir o fim do arquivo
        lido = fscanf(file,"%8[^,],%19[^,],%d,%199[^\r\n]",
            dados[records].data,
            dados[records].cod_cliente,
            &dados[records].cod_produto,
            dados[records].nome_produto);
        if(lido == 4) records++;

        if(lido != 4 && !feof(file)){
            printf("Formato do arquivo incorreto!\n");
            return 1;
        }

        if(ferror(file)){
            printf("Erro no arquivo!\n");
            return 1;
        }
    }
    fclose(file);
    printf("records lido:%d\n",records);
    
    for(int i = 0; i < records; i++){
          printf("Data: %s|Cod_cliente: %s |Cod_produto: %d |Nome_produto: %s\n",
        dados[i].data,
        dados[i].cod_cliente,
        dados[i].cod_produto,
        dados[i].nome_produto
    );
      
    }
    
    return 0;
}