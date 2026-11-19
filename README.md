# Sistema de Recomendação de Produtos (C++)

Sistema de recomendação baseado em **filtragem colaborativa**: clientes com hábitos de compra parecidos tendem a gostar dos mesmos produtos. A partir de um arquivo CSV de vendas, o programa descobre quais clientes são parecidos entre si e recomenda a um cliente os produtos que ele **ainda não comprou**, mas que clientes semelhantes compraram.

O projeto foi escrito em C/C++ e organizado em módulos independentes, com foco em **estruturas de dados**, **gerenciamento manual de memória** e **otimização de algoritmo**.

---

## Visão geral

```
CSV de vendas ──►  Leitura  ──►  Similaridade  ──►  Recomendação
                   (IDs e        (matriz de          (top-k produtos
                   listas de      distâncias entre    para um cliente)
                   compras)       clientes)
```

## Funcionalidades

- **Leitura e tratamento de CSV** com validação: linhas inválidas são descartadas e contadas, e o cabeçalho é ignorado.
- **Identificação de clientes e produtos únicos**, associando cada código original a um ID interno sequencial (usando `map`).
- **Lista de compras por cliente**, sem repetições, com os IDs dos produtos comprados.
- **Cálculo da similaridade entre todos os clientes** (distância de Jaccard), com **dois métodos** à escolha do usuário:
  - pela matriz transposta;
  - pelo método do somatório, otimizado (sem construir a transposta e aproveitando a simetria da matriz).
- **Medição do tempo de execução** de cada método, para comparação.
- **Busca do cliente mais similar** a um cliente consultado.
- **Recomendação dos `k` melhores produtos** para um cliente, exibindo nome, código e nota de cada produto.

## Como funciona

### 1. Leitura

Cada linha do CSV contém data, código do cliente, código do produto e nome do produto. Durante a leitura, o programa:

- cria um ID para cada cliente e para cada produto novo, na ordem em que aparecem (`map<string,int>` para clientes e `map<int,int>` para produtos);
- guarda os nomes e códigos em vetores, na posição do ID;
- monta, para cada cliente, a lista dos IDs dos produtos que ele comprou (`list<vector<int>>`), sem duplicar produtos.

### 2. Similaridade

Seja `A` a matriz de compras (linhas = clientes, colunas = produtos, `A[i][k] = 1` se o cliente `i` comprou o produto `k`).

**Matriz de interseção** `I = A × Aᵀ`: `I[i][j]` é o número de produtos que os clientes `i` e `j` compraram em comum.

**Distância entre clientes**:

```
S[i][j] = 1 - I[i][j] / |P_i|
```

onde `|P_i|` é o número de produtos comprados pelo cliente `i`. Distância **0** significa que `j` comprou tudo o que `i` comprou; **1** significa que não há nenhum produto em comum. Quanto menor o valor, mais parecidos são os clientes.

**Os dois métodos para calcular `I`:**

| | Método da transposta | Método do somatório (otimizado) |
|---|---|---|
| Fórmula | `I[i][j] = Σ A[i][k] × Aᵀ[k][j]` | `I[i][j] = Σ A[i][k] × A[j][k]` |
| Memória | cria uma cópia transposta de `A` | usa apenas `A` |
| Multiplicações | `n² × m` | cerca de `n² × m / 2` |
| Como | calcula todas as posições | calcula só `j ≥ i` e copia para a posição espelhada, pois `I` é simétrica |

Além de reduzir pela metade as multiplicações, o método do somatório lê as linhas de `A` em sequência, o que costuma ser mais amigável ao cache do processador.

### 3. Recomendação

Para um cliente `c` e um número `k`:

1. **Identificação de vizinhos:** monta a lista `L` com os clientes diferentes de `c` que têm distância menor que 1 (ou seja, ao menos um produto em comum).
2. **Inicialização do ranqueamento:** cria um vetor `R` com valor 1 para todos os produtos.
3. **Cálculo do ranqueamento:** para cada vizinho `v` em `L` e cada produto `p` comprado por `v` e **não** comprado por `c`, faz `R[p] = R[p] × S[c][v]`. Quanto mais vizinhos parecidos compraram um produto, menor (melhor) fica a nota dele.
4. **Ordenação e seleção:** descarta os produtos já comprados por `c` e os que não foram atualizados por nenhum vizinho, ordena os restantes do menor para o maior `R` (empate: menor ID) e devolve os `k` primeiros.

## Estrutura do projeto

```
.
├── main.cpp            # Fluxo principal: leitura → similaridade → recomendação
├── leitura.cpp/.h      # Leitura do CSV e construção das estruturas de dados
├── similaridade.cpp/.h # Matriz de interseção, distância de Jaccard e menu dos dois métodos
├── recomendacao.cpp/.h # Os 4 passos da recomendação, divididos em funções
├── alocar_matriz.cpp/.h# Alocação e liberação de matrizes (malloc/free)
└── dados/              # Arquivos CSV de vendas
```

## Como compilar e executar

Requisito: compilador C++ com suporte a C++11 ou superior (por exemplo, `g++`).

```bash
g++ -std=c++11 main.cpp leitura.cpp similaridade.cpp recomendacao.cpp alocar_matriz.cpp -o programa
./programa dados/dados_venda_cluster_0.csv
```

Ao executar, o programa pede a escolha do método de similaridade (`1` = transposta, `2` = somatório) e, em seguida, mostra o cliente mais similar, o tempo gasto e as recomendações.

O cliente consultado e a quantidade de recomendações são definidos no `main.cpp`:

```cpp
recomendacao(lista, 840, 5);   // índice do cliente, quantos produtos recomendar
```

## Exemplo de saída

Formato ilustrativo (os valores dependem do arquivo de dados):

```
Total de registros validos lidos: 1600
Total de linhas invalidas: 0

=== Resultado Etapa 2 (Similaridade) ===
Cliente consultado: Indice 740 (Codigo: ...)
Cliente mais similar: Indice 59 (Codigo: ...)
Distancia de Jaccard s(i,j): 0.0000
Tempo em segundos: 0.3849

Cliente consultado: Indice 840 (Codigo: ...)
1) <nome do produto> (codigo ..., indice ...) - R = 0.0000
2) <nome do produto> (codigo ..., indice ...) - R = 0.0000
...
```

## O que foi desenvolvido

- **Modularização:** o código foi dividido em módulos com responsabilidades claras (leitura, similaridade, recomendação e alocação de matrizes), cada um com seu arquivo de cabeçalho. O módulo de recomendação ainda foi subdividido em uma função para cada etapa do algoritmo.
- **Estruturas de dados:** uso combinado de `map` (tradução rápida de código para ID), `vector` e `list` para representar clientes, produtos e listas de compras.
- **Gerenciamento de memória:** matrizes e vetores auxiliares alocados dinamicamente com `malloc`/`free`, com definição clara de quem aloca e quem libera cada estrutura.
- **Otimização de algoritmo:** substituição do cálculo com matriz transposta por um método que usa a simetria da matriz de interseção, reduzindo as multiplicações e a memória usada.
- **Benchmark integrado:** o programa mede e exibe o tempo de cada método de similaridade, permitindo comparar as duas abordagens.
- **Tratamento de erros:** validação do arquivo de entrada, descarte e contagem de linhas inválidas, e verificação de índice de cliente na recomendação.
- **Depuração:** correção de erros de tipos, de declaração e de linkagem entre módulos, e de acessos fora dos limites de vetores e matrizes.

## Limitações e próximos passos

- O cliente consultado (índices fixos no código) deveria ser informado pelo usuário, de preferência pelo código original do cliente.
- A leitura processa no máximo 1.600 registros por arquivo.
- A matriz de distâncias ocupa memória proporcional a `n²`, o que limita o uso com bases muito grandes. Uma versão com matrizes esparsas ou com apenas os `k` vizinhos mais próximos resolveria isso.
- Falta uma suíte de testes automatizados.

## Autores

Thiago Dias e Hugo Suassuna
