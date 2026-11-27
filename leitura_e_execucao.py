import sys
from lista_compras import leitura
import solver


def main():
    if len(sys.argv) != 2:
        print(f"Uso: python {sys.argv[0]} <dados/dados_venda_cluster_0.csv>")
        return

    lista = leitura(sys.argv[1])

    n_cliente = len(lista.compras)
    n_prod = len(lista.codigo_produtos)
    cliente_alvo = 791
    k = 5

    if n_cliente == 0:
        print("Nenhum cliente carregado.")
        return

    print("\nDigite uma opcao:")
    print("[1] Similaridade pela transposta (Densa - Atividade 2)")
    print("[2] Similaridade pelo metodo do somatorio (Densa - Atividade 4)")
    print("[3] Similaridade por Matriz Esparsa CSR (Atividade 5)")
    opcao = int(input())

    mais_similar, dist, tempo, mem_densa, mem_csr, econ_compra = solver.similaridade(
        lista.compras, n_cliente, n_prod, cliente_alvo, opcao
    )

    if opcao in (1, 2):
        titulo = "PRIMEIRO METODO" if opcao == 1 else "SEGUNDO METODO"
        print(f"\n=== Resultado Etapa 2 (Similaridade) ===")
        print(f"===== {titulo} =======")
        if mais_similar != -1:
            print(f"Cliente consultado: Indice {cliente_alvo} (Codigo: {lista.clientes[cliente_alvo]})")
            print(f"Cliente mais similar: Indice {mais_similar} (Codigo: {lista.clientes[mais_similar]})")
            print(f"Distancia de Jaccard s(i,j): {dist:.4f}")
            print(f"Tempo em segundos: {tempo:.4f}")
            print(f"Memoria Similaridade Densa: {mem_densa:.2f} KB")
    elif opcao == 3:
        print("\n=== Resultado Similaridade (Matriz Esparsa CSR) ===")
        if mais_similar != -1:
            print(f"Cliente consultado: Indice {cliente_alvo} (Codigo: {lista.clientes[cliente_alvo]})")
            print(f"Cliente mais similar: Indice {mais_similar} (Codigo: {lista.clientes[mais_similar]})")
            print(f"Distancia de Jaccard s(i,j): {dist:.4f}")
        print(f"Tempo CSR: {tempo:.6f} s")
        print(f"Memoria Compras: Densa = {mem_densa:.2f} KB | CSR = {mem_csr:.2f} KB | Economia = {econ_compra:.2f}%")

    # Recomendacao (k = 5 para o cliente 791)
    rec = solver.recomendacao(lista.compras, n_cliente, n_prod, cliente_alvo, k)

    print("\n=== Recomendacoes (via Matriz Esparsa CSR) ===")
    print(f"Cliente consultado: Indice {cliente_alvo} (Codigo: {lista.clientes[cliente_alvo]})")
    if not rec:
        print("Nenhum produto para recomendar.")
    for i, (id_prod, valor_r) in enumerate(rec, start=1):
        print(f"{i}) Nome do produto: {lista.nome_produtos[id_prod]} (indice {id_prod}) - R = {valor_r:.4f}")


if __name__ == "__main__":
    main()