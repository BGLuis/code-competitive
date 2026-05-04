# Problema B - Balões Equilibrados

Um grupo de estudantes está fundando um clube de programação competitiva e eles esperam que $N$ pessoas entrem, uma a uma. Quando cada pessoa entra, ela deve trazer entre 1 e $K$ balões.

Os fundadores querem garantir que as coisas sejam justas e decidiram impor a seguinte regra de justiça: após a entrada de cada pessoa, deve ser possível redistribuir igualmente todos os balões arrecadados até o momento entre os membros atuais. Em outras palavras, após cada passo, o número total de balões até o momento deve ser divisível pelo número atual de pessoas.

Sendo programadores competitivos, eles perceberam que isso dá um belo problema de contagem e decidiram desafiá-lo. Quantas sequências de $N$ contribuições de balões (cada contribuição entre 1 e $K$) satisfazem a regra de justiça a cada passo?

Duas sequências são consideradas diferentes se diferem em qualquer posição.

## Entrada

A entrada consiste em uma única linha que contém dois inteiros $N$ ($1 \le N \le 10^9$) e $K$ ($1 \le K \le 2000$), indicando respectivamente o número de membros e o número máximo de balões que cada membro pode trazer.

## Saída

Imprima uma única linha com um inteiro indicando o número de sequências de contribuições de balões que satisfazem a regra de justiça. Como este número pode ser muito grande, imprima o resto da divisão dele por $998244353$.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `3 3`                | `5`                |

**Explicação do exemplo 1:**
As 5 sequências de contribuições de balões que respeitam a regra de justiça são:
$[1, 1, 1]$,
$[1, 3, 2]$,
$[2, 2, 2]$,
$[3, 1, 2]$,
$[3, 3, 3]$.

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `4 1`                | `1`                |

**Explicação do exemplo 2:**
Todos devem contribuir com um único balão, então a única sequência possível é $[1, 1, 1, 1]$.