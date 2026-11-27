import csv


class ListaCompras:
    def __init__(self):
        self.codigo_clientes = []
        self.nome_produtos = []
        self.codigo_produtos = []
        self.codigo_prod = {}
        self.codigo = {}
        self.clientes = []
        self.compras = []


def leitura(caminho_arquivo: str) -> ListaCompras:
    lista = ListaCompras()
    total = 0
    lixo = 0
    qtd_cliente_unico = 0
    qtd_repetido_nome = 0
    qtd_repetido_produto = 0

    with open(caminho_arquivo, mode="r", encoding="latin-1") as arquivo:
        leitor = csv.reader(arquivo)
        next(leitor, None)  # Descarta o cabecalho inicial

        for linha in leitor:
            if total >= 1600:
                break

            if len(linha) < 4:
                lixo += 1
                continue

            try:
                cod_cliente = linha[1].strip()
                produto = int(linha[2].strip())
                nome_prod = linha[3].strip()
            except ValueError:
                lixo += 1
                continue

            total += 1

            # Controla clientes unicos
            if cod_cliente not in lista.codigo:
                lista.codigo[cod_cliente] = qtd_cliente_unico
                lista.codigo_clientes.append(cod_cliente)
                lista.clientes.append(cod_cliente)
                lista.compras.append([])
                qtd_cliente_unico += 1

            # Controla produtos unicos
            if produto not in lista.codigo_prod:
                lista.codigo_prod[produto] = qtd_repetido_produto
                lista.codigo_produtos.append(produto)
                lista.nome_produtos.append(nome_prod)
                qtd_repetido_produto += 1
                qtd_repetido_nome += 1

            # Adiciona produto na lista de compras do cliente sem duplicar
            pos_cliente = lista.codigo[cod_cliente]
            id_prod = lista.codigo_prod[produto]

            if id_prod not in lista.compras[pos_cliente]:
                lista.compras[pos_cliente].append(id_prod)

    print(f"\nTotal de registros validos lidos: {total}")
    print(f"Total de linhas invalidas: {lixo}")
    print(f"Total de produtos unicos: {qtd_repetido_nome}")
    print(f"Total de clientes unicos: {qtd_cliente_unico}")

    return lista