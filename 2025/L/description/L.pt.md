# Problema L - Criaturas Solitárias

Existem $N$ criaturas, numeradas de 1 a $N$, que vivem em uma cidade infinita representada pelo plano 2D. A criatura $i$ vive na linha reta $y = M_i \cdot x + C_i$; ela pode se mover para qualquer lugar ao longo dessa linha. Não existem duas criaturas que compartilhem a mesma linha, mas um encontro ocorre se suas linhas se cruzarem. Nesse caso, o ponto de interseção é chamado de ponto de encontro das duas criaturas.

A cidade acaba de construir um novo parquinho. A fronteira do parquinho é a parábola (alinhada aos eixos) $y = A \cdot x^2 + B$, onde $A > 0$. O parquinho é a região aberta $y > A \cdot x^2 + B$, de modo que os pontos exatamente na fronteira não estão dentro do parquinho.

Agora, as autoridades da cidade querem saber se o novo parquinho está reduzindo a solidão, e é aí que sua ajuda é necessária. Sua tarefa é contar quantos pares distintos de criaturas possuem um ponto de encontro que esteja estritamente dentro do parquinho. Cada par é contado uma única vez; a ordem das criaturas em um par não importa.

## Entrada

A primeira linha contém três inteiros $N$ ($2 \le N \le 10^5$), $A$ ($1 \le A \le 10^4$) e $B$ ($-10^4 \le B \le 10^4$), indicando que existem $N$ criaturas e que o parquinho é a região aberta do plano 2D $y > A \cdot x^2 + B$. Cada criatura é identificada por um número inteiro distinto de 1 a $N$.

A $i$-ésima das próximas $N$ linhas contém dois inteiros $M_i$ e $C_i$ ($-10^4 \le M_i, C_i \le 10^4$), representando que a criatura $i$ vive na linha reta $y = M_i \cdot x + C_i$. É garantido que não existem duas linhas idênticas.

## Saída

Imprima uma única linha com um inteiro indicando o número de pares distintos de criaturas cujo ponto de encontro esteja estritamente dentro do parquinho.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `3 1 0`<br>`-1 -1`<br>`2 4`<br>`2 0` | `0`                |

**Explicação do exemplo 1:**
A criatura 1 encontra as criaturas 2 e 3, mas em ambos os casos o ponto de encontro fica fora do parquinho. Não há outros pontos de encontro, então a resposta é 0.

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `4 1 -2`<br>`-1 0`<br>`0 1`<br>`1 0`<br>`2 1` | `5`                |

**Explicação do exemplo 2:**
Cada par de criaturas possui um ponto de encontro. No entanto, o ponto de encontro das criaturas 3 e 4 fica na fronteira do parquinho ($y = A \cdot x^2 + B$), portanto esse par é excluído da contagem.

| Exemplo de entrada 3 | Exemplo de saída 3 |
|:---------------------|:-------------------|
| `4 1 0`<br>`7 -3`<br>`5 -2`<br>`3 -1`<br>`1 0` | `6`                |