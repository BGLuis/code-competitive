# Problema C - Ruas Limpas

Mais um ano de Virada Cultural em São Paulo. A cidade está cheia de gente e as ruas cheias de vida. Pessoas de toda a cidade vão às ruas para curtir uma noite de eventos culturais e muita música.

Oswaldo, um funcionário da prefeitura, recebeu a tarefa de contratar faxineiros para limpar as $S$ ruas de São Paulo em não mais que $K$ horas após este grande evento. Seu chefe lhe deu uma lista de $N$ faxineiros que ele poderia contratar, numerados de 1 a $N$, junto com informações sobre a rapidez com que trabalham e o pagamento que aceitam. De acordo com a lista, o faxineiro $i$ pode limpar qualquer rua em $H_i$ horas e aceita como pagamento qualquer valor entre $L_i$ e $U_i$ para limpar cada rua.

Oswaldo deve contratar um subconjunto $C$ de faxineiros que estão na lista. Para cada faxineiro contratado $i \in C$, ele deve atribuir $s_i$ ruas para eles limparem e um pagamento $p_i$ por rua, levando em consideração as seguintes diretrizes:
* Cada número de ruas $s_i$ deve ser um inteiro positivo e a soma $\sum_{i \in C} s_i$ deve ser exatamente $S$, pois cada rua deve ser limpa e nenhuma rua pode ser limpa por mais de um faxineiro.
* Note que cada faxineiro contratado $i$ levará $s_i \cdot H_i$ horas para limpar as ruas designadas a ele. Como os faxineiros podem trabalhar em paralelo, o trabalho de limpeza levará, no total, $\max_{i \in C} (s_i \cdot H_i)$ horas para ser concluído. Este tempo total deve ser no máximo $K$ horas.
* O pagamento por rua $p_i$ deve ser um número racional entre $L_i$ e $U_i$ (ou seja, $L_i \le p_i \le U_i$).
* Para garantir um processo de contratação justo, o pagamento por hora de trabalho $p_i / H_i$ deve ser o mesmo para todos os faxineiros contratados.

Note que as restrições acima não se aplicam aos faxineiros que não foram contratados.

O faxineiro contratado $i$ receberá $s_i \cdot p_i$ como pagamento, sendo o pagamento total $\sum_{i \in C} s_i \cdot p_i$. Ajude Oswaldo a determinar o pagamento total mínimo para limpar as ruas respeitando as diretrizes fornecidas, ou diga a ele que esses requisitos não podem ser satisfeitos.

## Entrada

A primeira linha contém três inteiros $N$, $S$ e $K$ ($1 \le N, S \le 10^5$ e $1 \le K \le 10^9$), indicando respectivamente o número de faxineiros disponíveis, o número de ruas a serem limpas e em quantas horas o trabalho deve ser concluído. Cada faxineiro é identificado por um inteiro distinto de 1 a $N$.

A $i$-ésima das próximas $N$ linhas descreve o faxineiro $i$ com três inteiros $H_i$ ($1 \le H_i \le 10^5$), $L_i$ e $U_i$ ($1 \le L_i \le U_i \le 100$), indicando que o faxineiro pode limpar qualquer rua em $H_i$ horas, e aceita como pagamento qualquer valor no intervalo $[L_i, U_i]$.

## Saída

Se as diretrizes fornecidas puderem ser satisfeitas, imprima uma única linha com dois inteiros positivos $x$ e $y$, de modo que $x/y$ seja uma fração irredutível indicando o pagamento total mínimo para limpar as ruas de acordo com essas diretrizes.
Se os requisitos não puderem ser atendidos, imprima uma linha com o caractere `*` (asterisco) em seu lugar.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `2 15 10`<br>`1 4 10`<br>`2 2 8` | `80 1`             |

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `2 7 9`<br>`3 4 10`<br>`2 2 8` | `68 3`             |

| Exemplo de entrada 3 | Exemplo de saída 3 |
|:---------------------|:-------------------|
| `2 15 10`<br>`1 4 10`<br>`5 2 8` | `*`                |