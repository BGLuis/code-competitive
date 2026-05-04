# Problema G - Medidor de Energia de Gridoland

Gridoland é uma cidade que consiste em uma grade de blocos de $N \times M$, projetada para uma distribuição robusta de energia. O suprimento de energia da cidade é uma represa que pode efetivamente gerar uma corrente arbitrariamente grande, e ela está conectada ao bloco superior esquerdo, nas coordenadas $(1, 1)$. A prefeita está planejando sediar uma grande celebração que consumirá muita energia na prefeitura, que está localizada nas coordenadas $(N, M)$, e ela o contratou para ajudar o comitê de planejamento.

Cada bloco em Gridoland tem seu próprio transformador de energia. Cada transformador tem uma capacidade inicial de passagem de corrente, mas essa capacidade muda linearmente com o tempo a uma taxa possivelmente diferente para cada transformador. Para ajudar no seu trabalho, os técnicos determinaram a capacidade inicial de cada transformador $C_{i,j}$ e sua taxa de mudança $R_{i,j}$ (que pode ser positiva, negativa ou zero). Portanto, você sabe que a capacidade do transformador no bloco $(i, j)$ no minuto $t$ será $C_{i,j} + t \cdot R_{i,j}$.

A corrente flui dentro da cidade graças a vários cabos de energia. Todos os cabos de energia em Gridoland são feitos de um material supercondutor que pode passar uma corrente arbitrariamente alta em qualquer direção. A própria prefeita lhe entregou o mapa de conectividade da cidade, contendo todos os pares de blocos adjacentes (vertical ou horizontalmente) que estão conectados por um cabo de energia.

Durante a celebração, não haverá consumo de energia em nenhum bloco, exceto na prefeitura, portanto, toda a corrente que entra na rede de energia será usada para a celebração. A celebração deve acontecer no máximo em $K$ minutos no futuro, pois o mandato da prefeita terminará nesse momento.

Você deve responder à seguinte pergunta para o comitê de planejamento: qual será a capacidade máxima da rede para suprir a celebração em um número inteiro de minutos $t$ no intervalo $[0, K]$?

A cada momento, a capacidade da rede do ponto de vista da prefeitura é igual à quantidade máxima de corrente que pode fluir do suprimento de energia para a prefeitura. A corrente que entra em cada transformador é limitada pela capacidade daquele transformador e pela restrição de que toda a corrente que entra em um bloco tem que sair (exceto pela prefeitura, que durante a celebração consumirá toda a energia que receber).

## Entrada

A primeira linha contém quatro inteiros $N$, $M$, $P$ e $K$ ($2 \le N, M \le 300$, $P \ge 0$ e $0 \le K \le 10^{12}$). Os valores $N$ e $M$ indicam as dimensões da cidade, $P$ representa o número de cabos de energia no mapa de conectividade da cidade e $K$ é o número de minutos restantes no mandato da prefeita.

Cada uma das $N$ linhas seguintes contém $M$ inteiros. Na $i$-ésima linha, o $j$-ésimo inteiro é $C_{i,j}$, a capacidade inicial do transformador no bloco $(i, j)$ ($0 \le C_{i,j} \le 10^{12}$ para $i = 1, 2, \ldots, N$ e $j = 1, 2, \ldots, M$).

Cada uma das $N$ linhas seguintes contém $M$ inteiros. Na $i$-ésima linha, o $j$-ésimo inteiro é $R_{i,j}$, a taxa de mudança do transformador no bloco $(i, j)$ ($-10^6 \le R_{i,j} \le 10^6$ para $i = 1, 2, \ldots, N$ e $j = 1, 2, \ldots, M$).

Cada uma das próximas $P$ linhas descreve um cabo de energia bidirecional com quatro inteiros $X_1$, $Y_1$, $X_2$ e $Y_2$ ($1 \le X_1, X_2 \le N$ e $1 \le Y_1, Y_2 \le M$), indicando que o cabo conecta o bloco $(X_1, Y_1)$ com o bloco $(X_2, Y_2)$.

É garantido que todos os transformadores tenham capacidade não negativa a cada momento no intervalo $[0, K]$, ou seja, $C_{i,j} + t \cdot R_{i,j} \ge 0$ para todo bloco $(i, j)$ e todo $t \in [0, K]$.

É garantido que cada cabo conecte um par diferente de blocos e que quaisquer dois blocos conectados por um cabo compartilhem um lado.

## Saída

Imprima uma única linha com um inteiro indicando a capacidade máxima da rede para suprir a celebração em um número inteiro de minutos $t$ no intervalo $[0, K]$.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `2 2 3 10`<br>`5 4`<br>`5 6`<br>`0 0`<br>`0 0`<br>`2 1 1 1`<br>`1 1 1 2`<br>`1 2 2 2` | `4`                |

**Explicação do exemplo 1:**
Neste caso, a cidade é uma grade de $2 \times 2$, $K = 10$, e todos os blocos têm um transformador com capacidade estável porque $R_{i,j} = 0$. O bloco $(1, 1)$ conectado à fonte de alimentação possui um transformador de capacidade $C_{1,1} = 5$. Está ligado ao transformador do bloco $(1, 2)$ que tem capacidade $C_{1,2} = 4$. Este transformador está ligado ao transformador da prefeitura no bloco $(2, 2)$, que tem capacidade $C_{2,2} = 6$. A qualquer momento entre $t = 0$ e $t = K$, a rede de energia pode suprir a prefeitura com uma corrente máxima de 4.

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `2 3 6 12`<br>`25 1 18`<br>`10 2 50`<br>`0 2 -1`<br>`1 0 -2`<br>`1 1 1 2`<br>`1 2 1 3`<br>`1 3 2 3`<br>`1 1 2 1`<br>`2 1 2 2`<br>`2 2 2 3` | `14`               |

**Explicação do exemplo 2:**
Aqui a cidade é uma grade $2 \times 3$, com $K = 12$. Existem dois caminhos para que a energia flua do bloco $(1, 1)$ até a prefeitura em $(2, 3)$. Um deles usa os cabos da linha superior $(1, 1) \rightarrow (1, 2) \rightarrow (1, 3) \rightarrow (2, 3)$, e o outro usa os cabos da linha inferior $(1, 1) \rightarrow (2, 1) \rightarrow (2, 2) \rightarrow (2, 3)$. O momento ideal para a comemoração é em $t = 6$, quando uma corrente de 12 pode passar pelo caminho superior, e uma corrente adicional de 2 pode passar pelo caminho inferior.

| Exemplo de entrada 3 | Exemplo de saída 3 |
|:---------------------|:-------------------|
| `2 2 1 1000000000000`<br>`5 4`<br>`5 6`<br>`0 0`<br>`0 0`<br>`2 1 1 1` | `0`                |