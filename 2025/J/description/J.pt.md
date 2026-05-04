# Problema J - Multidão Julgadora

Depois de uma noite gloriosa de comédia stand-up, um comediante quer medir o seu sucesso. Não com pesquisas, não com curtidas, mas com sons reais da multidão! Felizmente, o microfone capturou todas as reações durante a apresentação. O áudio foi convertido em uma única string $S$ cheia de sons misturados, como aplausos, risadas, vaias, etc.

O comediante usa uma ferramenta poderosa para avaliar seu show: o Calculador de Performance Instantânea de Comediante (ICPC). O ICPC é capaz de analisar $S$ e calcular a pontuação emocional da performance, que reflete o quão bem o comediante se saiu no palco.

A pontuação é baseada na ocorrência de três substrings específicas dentro da string $S$:
* cada ocorrência de "ha" aumenta a pontuação em 1,
* cada ocorrência de "boooo" diminui a pontuação em 1, e
* cada ocorrência de "bravo" aumenta a pontuação em 3.

Quaisquer outras substrings em $S$ (como "ahhh", "woohoo" ou sons aleatórios) são ignoradas e não afetam a pontuação. Sua missão é simular o ICPC calculando a pontuação emocional da performance.

## Entrada

A entrada consiste em uma única linha contendo uma string $S$ ($1 \le |S| \le 1000$) composta por letras minúsculas, representando a sequência de sons da multidão captados pelo microfone.

## Saída

Imprima uma única linha com um número inteiro indicando a pontuação emocional da performance.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `boooohaboooo`       | `-1`               |

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `brhavo`             | `1`                |

| Exemplo de entrada 3 | Exemplo de saída 3 |
|:---------------------|:-------------------|
| `booo`               | `0`                |

| Exemplo de entrada 4 | Exemplo de saída 4 |
|:---------------------|:-------------------|
| `bravoooooobraboooooo` | `2`                |

| Exemplo de entrada 5 | Exemplo de saída 5 |
|:---------------------|:-------------------|
| `buuuuuuthisshowisawfulweshallgonow` | `1`                |