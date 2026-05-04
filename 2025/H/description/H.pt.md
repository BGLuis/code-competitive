# Problema H - Horizontes Mais Difíceis

Lola mantém uma longa lista de tarefas que ela precisa terminar. Cada tarefa tem um número de dificuldade (quanto maior o número, mais difícil a tarefa), e as tarefas devem ser concluídas na ordem em que aparecem.

A lista continua crescendo porque os itens raramente são removidos. Cansada disso, Lola decidiu agir: ela quer dividir sua lista em vários grupos consecutivos de tarefas, onde cada grupo representa o trabalho que ela fará em um dia.

Para se manter motivada, Lola quer sentir que cada dia é um passo à frente. Portanto, a partir do segundo dia, a tarefa mais difícil daquele dia deve ser estritamente mais difícil do que a tarefa mais difícil do dia anterior.

Lola também não quer trabalhar demais em um único dia, então ela quer distribuir o trabalho pelo maior número de dias possível.

Qual é o número máximo de dias em que ela pode dividir as tarefas sob estas regras?

## Entrada

A primeira linha contém um inteiro $N$ ($1 \le N \le 10^5$) indicando o número de tarefas.

A próxima linha contém $N$ inteiros $D_1, D_2, \dots, D_N$ ($1 \le D_i \le 10^5$ para $i = 1, 2, \dots, N$), representando as dificuldades das tarefas na ordem em que aparecem na lista.

## Saída

Imprima uma única linha com um inteiro indicando o número máximo de dias em que Lola pode dividir as tarefas, de modo que a tarefa mais difícil em cada dia seja estritamente mais difícil do que a tarefa mais difícil do dia anterior.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `5`<br>`1 2 1 3 4`   | `4`                |

**Explicação do exemplo 1:**
Uma maneira ideal de dividir as tarefas em 4 dias é: `1 | 2 1 | 3 | 4` com dificuldades estritamente crescentes das tarefas mais difíceis: $1 < 2 < 3 < 4$.

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `6`<br>`2 1 4 2 3 5` | `3`                |

**Explicação do exemplo 2:**
Aqui, uma maneira ideal de dividir a lista em 3 dias é: `2 1 | 4 2 | 3 5` com $2 < 4 < 5$.