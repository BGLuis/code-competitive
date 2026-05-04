# Problema D - Exibindo Decimais

Depois que o grupo BRICS introduziu a Moeda de Pagamento Comum Internacional (ICPC), todas as moedas (como real, peso, dólar, etc.) agora são definidas por seus valores em ICPCs.

Para ajudar as pessoas a navegar nesta nova ordem mundial, você decidiu criar um site para calcular as taxas de conversão entre moedas. O site lida com $N$ moedas, numeradas de 1 a $N$. Cada moeda $i$ tem um valor inteiro associado $A_i$, indicando que uma unidade da moeda $i$ vale $A_i$ ICPCs.

O site funciona da seguinte forma: o usuário seleciona uma moeda de origem $i$, em seguida, ele seleciona uma moeda de destino $j$ (pode ser que $j = i$), e o site exibe a taxa de conversão entre elas, ou seja, a representação decimal exata de $A_i / A_j$.

Para lidar com dízimas periódicas, você decidiu exibir apenas o primeiro período (representado por uma linha acima dele, quando existir). Assim, o quociente $4/4 = 1$ requer a exibição de 1 dígito, $4/3 = 1.\overline{3}$ precisa de 2 dígitos, $41/4 = 10.25$ usa 4 dígitos e $3/14 = 0.2\overline{142857}$ usa 8 dígitos.

Seu site está no ar com sucesso há algum tempo. Como parte de sua análise de desempenho, você deseja calcular o tamanho de exibição esperado das páginas de taxa de conversão.

Dados os valores $A_1, A_2, \ldots, A_N$, você deve calcular o número esperado de dígitos necessários para exibir o quociente $A_i / A_j$, quando $i$ e $j$ são escolhidos uniformemente ao acaso entre 1 e $N$.

## Entrada

A primeira linha contém um inteiro $N$ ($1 \le N \le 10^5$) indicando o número de moedas.
A segunda linha contém $N$ inteiros $A_1, A_2, \ldots, A_N$ ($1 \le A_i \le 10^5$ para $i = 1, 2, \ldots, N$), onde $A_i$ é o valor de uma unidade da moeda $i$ expressa em ICPCs.

## Saída

O número esperado de dígitos necessários para exibir o quociente $A_i / A_j$, quando $i$ e $j$ são escolhidos uniformemente ao acaso entre 1 e $N$, pode ser expresso como uma fração irredutível $P/Q$, com $Q$ e $M = 998244353$ sendo coprimos. Seja $Q^{-1}$ o inverso multiplicativo modular de $Q$ módulo $M$, isto é, $Q \cdot Q^{-1} \equiv 1 \pmod M$. Imprima uma única linha com o inteiro $P \cdot Q^{-1} \bmod M$.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `3`<br>`15 36 14`    | `332748121`        |

**Explicação do exemplo 1:**
A tabela abaixo mostra o número de dígitos necessários para exibir os quocientes linha/coluna, cada um deles com probabilidade de $1/9$.

| | 15 | 36 | 14 |
|---|---|---|---|
| **15** | 1 | 4 | 8 |
| **36** | 2 | 1 | 7 |
| **14** | 3 | 3 | 1 |

Por exemplo, $14/36 = 0.3\overline{8}$ requer 3 dígitos. O número esperado de dígitos é $30/9 = 10/3$, ou seja, $P = 10$ e $Q = 3$. Como $Q^{-1} = 332748118$, a resposta final a ser impressa é $P \cdot Q^{-1} \bmod M = 10 \cdot 332748118 \bmod 998244353 = 332748121$.