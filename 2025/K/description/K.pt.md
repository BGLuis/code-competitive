# Problema K - Conquista dos Reis

Existem $N$ reis de xadrez colocados em uma grade infinita. Cada um deles ocupa uma célula diferente. A área conquistada da grade é definida como o número de células na menor subgrade retangular que contém todas as células ocupadas pelos reis.

Você tem permissão para realizar uma sequência de $K$ movimentos. Em cada movimento, você seleciona qualquer rei e o move para qualquer uma de suas oito células adjacentes, como um rei normal se move no xadrez. Não podem existir dois reis ocupando a mesma célula ao mesmo tempo.

Qual é a maior área conquistada que pode ser alcançada após a realização de $K$ movimentos?

## Entrada

A primeira linha contém dois inteiros $N$ e $K$ ($1 \le N, K \le 10^5$).

Cada uma das próximas $N$ linhas contém dois inteiros $R$ e $C$ ($-10^6 \le R, C \le 10^6$), indicando que um rei ocupa a célula na linha $R$ e coluna $C$. É garantido que todos os reis ocupam células diferentes.

## Saída

Imprima uma única linha com um inteiro representando a maior área conquistada que pode ser alcançada após a realização de $K$ movimentos.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `4 1`<br>`1 -1`<br>`-2 -1`<br>`0 -2`<br>`0 0` | `16`               |

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `2 3`<br>`1 1`<br>`-1 0` | `30`               |

| Exemplo de entrada 3 | Exemplo de saída 3 |
|:---------------------|:-------------------|
| `2 99999`<br>`0 0`<br>`1 1` | `10000200001`      |