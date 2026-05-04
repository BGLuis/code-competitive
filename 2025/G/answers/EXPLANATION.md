# Problema G - Medidor de Energia de Gridoland

## Descrição do Problema
O objetivo é determinar o fluxo máximo de energia de um suprimento no bloco (1, 1) até a prefeitura no bloco (N, M) em uma grade de $N \times M$. Cada bloco possui um transformador cuja capacidade varia linearmente com o tempo: $f_{i,j}(t) = C_{i,j} + t \cdot R_{i,j}$. Os cabos que conectam os blocos têm capacidade infinita. Devemos encontrar o tempo inteiro $t \in [0, K]$ que maximiza o fluxo total.

## Análise Matemática
1. **Corte Mínimo de Nós**: Como as conexões (arestas) são infinitas, o fluxo máximo é determinado pelo corte mínimo de nós (transformadores). Isso significa encontrar um conjunto de transformadores cuja soma das capacidades seja mínima e que desconecte a fonte (1, 1) do destino (N, M).
2. **Propriedade de Concavidade**: A capacidade de cada nó é uma função linear de $t$. O fluxo máximo $F(t)$, definido como o mínimo entre todos os possíveis cortes (cada um sendo uma soma de funções lineares), é uma função **côncava**. 
3. **Dualidade em Grafos Planares**: Para uma grade planar, o corte mínimo pode ser modelado como o caminho mais curto em um grafo dual construído adequadamente.

## Estratégia de Solução
1. **Construção do Grafo Dual**:
   - Cada face da grade e as regiões externas são representadas como nós no grafo dual.
   - Definimos dois super-nós externos: $S^*$ (região acima e à direita da grade) e $T^*$ (região abaixo e à esquerda).
   - Para cada transformador no bloco $(i, j)$, criamos uma estrutura no dual que exige o pagamento de $w_{i,j}(t)$ para transitar entre as faces adjacentes a esse bloco.
   - Arestas entre blocos adjacentes que **não** possuem cabos são representadas no dual como caminhos de peso 0, facilitando o corte da rede.
2. **Algoritmo**:
   - Utilizamos **Busca Ternária** para encontrar o $t$ que maximiza $F(t)$ no intervalo $[0, K]$.
   - Para cada valor de $t$ testado, calculamos $F(t)$ através do algoritmo de **Dijkstra** no grafo dual.
3. **Precisão e Escala**:
   - As capacidades e o tempo podem atingir $10^{12}$, resultando em fluxos que superam a capacidade de um `long long` de 64 bits. Por isso, utilizamos `__int128_t` no C++ para os cálculos de distância e fluxo.

## Complexidade
- **Tempo**: $O(\log K \cdot NM \log(NM))$. Com $N, M = 300$, o grafo dual possui aproximadamente 180.000 nós, e a busca ternária realiza cerca de 70 iterações de Dijkstra.
- **Espaço**: $O(NM)$ para a estrutura de adjacência do grafo dual.
