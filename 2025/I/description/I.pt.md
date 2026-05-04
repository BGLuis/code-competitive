# Problema I - Rota de Infiltração

Tomás é um agente secreto tentando se infiltrar no quartel-general super-seguro do Consórcio Internacional de Programação Competitiva (ICPC). O edifício tem $2N$ andares, numerados de 1 a $2N$. Começando no andar 1, ele deve chegar ao andar $2N$ para roubar algoritmos confidenciais e depois fazer uma fuga dramática de paraquedas do telhado.

Ele obteve $M$ códigos de acesso de elevadores roubados. Cada código de acesso conecta dois andares específicos $S$ e $T$ ($S < T$) e pode ser usado para forçar o elevador a se mover para cima do andar $S$ para o andar $T$.

O edifício possui um sistema de segurança avançado composto por $N$ sensores. O sensor $i$ monitora os andares $i$ e $i + N$ (para $1 \le i \le N$). Quando Tomás usa um código de elevador para sair de um andar, o sensor de monitoramento detecta a intrusão e sela permanentemente ambos os andares que monitora. Andares selados não podem ser acessados e os códigos não podem ser usados a partir de andares selados. Assim, ele pode entrar em no máximo um andar de cada par monitorado.

Tomás precisa da sua ajuda para garantir o sucesso da missão. Sua tarefa é determinar uma rota de infiltração, que é uma sequência de andares $f_1, f_2, \dots, f_k$, tal que:
* $f_1 = 1$,
* $f_k = 2N$,
* existe um código de acesso que permite mover-se do andar $f_i$ para o andar $f_{i+1}$ para $1 \le i \le k - 1$, e
* nenhum par de andares $f_i$ e $f_j$ na sequência (com $i \ne j$) é monitorado pelo mesmo sensor.

Se tal rota de infiltração existir, imprima-a. Caso contrário, indique que a missão é impossível.

## Entrada

A primeira linha contém dois inteiros $N$ ($1 \le N \le 500$) e $M$ ($1 \le M \le 1000$), indicando respectivamente que o edifício tem $2N$ andares e que Tomás possui $M$ códigos de acesso. Os andares são identificados por inteiros distintos de 1 a $2N$.

Cada uma das próximas $M$ linhas descreve um código de acesso com dois inteiros $S$ e $T$ ($1 \le S < T \le 2N$), representando que o código de acesso permite o movimento do andar $S$ para o andar $T$.

## Saída

Se uma rota de infiltração existir, imprima duas linhas. A primeira linha deve conter um número inteiro $k$ indicando o número de andares na rota. A segunda linha deve conter $k$ inteiros $f_1, f_2, \dots, f_k$ representando os andares incluídos na rota. Se houver múltiplas soluções, imprima qualquer uma delas.

Se a missão for impossível, imprima uma linha com o caractere `*` (asterisco).

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `1 1`<br>`1 2`       | `*`                |

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `4 9`<br>`1 2`<br>`2 3`<br>`3 6`<br>`6 7`<br>`7 8`<br>`1 3`<br>`3 7`<br>`2 6`<br>`6 8` | `4`<br>`1 3 6 8` |

| Exemplo de entrada 3 | Exemplo de saída 3 |
|:---------------------|:-------------------|
| `4 8`<br>`1 2`<br>`1 3`<br>`2 3`<br>`2 6`<br>`3 7`<br>`6 7`<br>`6 8`<br>`7 8` | `*`                |