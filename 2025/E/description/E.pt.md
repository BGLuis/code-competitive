# Problema E - Rações de Emergência

Uma vila armazena caixas de rações de emergência. Cada caixa contém um certo número de rações, e o líder da vila acompanha quantas rações existem em cada caixa. Inicialmente, não há caixas no suprimento. Às vezes, uma nova caixa é adicionada ao suprimento ou uma caixa existente é removida.

Em uma emergência, os aldeões devem escolher exatamente uma das seguintes ações a cada dia que a emergência durar:
1. escolher uma caixa não vazia e usar todas as rações daquela caixa, ou
2. usar uma ração de cada caixa não vazia.

O líder está preocupado com a possibilidade de consumo rápido. Portanto, após cada mudança no suprimento, o líder quer saber quantos dias seriam necessários antes que todas as caixas ficassem vazias, assumindo que uma emergência ocorra e os aldeões escolham ações que esvaziem todas as caixas no menor número de dias possível.

Note que as rações não são realmente usadas; o líder só quer saber o número mínimo de dias necessários para esvaziar todas as caixas. Como executar essas simulações não é algo que o líder pode fazer sozinho, você foi convidado a ajudar.

## Entrada

A primeira linha contém um inteiro $Q$ ($1 \le Q \le 3 \cdot 10^5$) indicando o número de mudanças no suprimento.

A próxima linha contém $Q$ inteiros com sinal $X_1, X_2, \ldots, X_Q$ ($1 \le |X_i| \le 10^9$ para $i = 1, 2, \ldots, Q$) indicando, em ordem cronológica, a adição e remoção de caixas no suprimento. Um valor positivo $X_i = +c$ significa que uma caixa contendo $c$ rações é adicionada, enquanto um valor negativo $X_i = -c$ significa que uma caixa contendo exatamente $c$ rações é removida.

É garantido que cada remoção é válida, ou seja, sempre que ocorre uma remoção $X_i = -c$, há pelo menos uma caixa no suprimento que contém exatamente $c$ rações.

## Saída

Imprima uma única linha com $Q$ inteiros, de modo que o $i$-ésimo inteiro indique o número mínimo de dias necessários para usar todas as rações logo após a $i$-ésima mudança no suprimento.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `8`<br>`+1 +2 +2 +7 -2 +10 -1 -2` | `1 2 2 3 3 4 3 2` |

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `2`<br>`+42 -42`     | `1 0`              |