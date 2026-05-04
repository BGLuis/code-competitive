# Problema A - Torta de Maçã

## Descrição do Problema
O objetivo é determinar se uma sequência parcial de inteiros (dividida em prefixo $L$ e sufixo $R$) pode ser completada para formar uma sequência válida para um dado $N$. Uma sequência é válida se cada conjunto de dois inteiros distintos entre $1$ e $N$ aparece exatamente uma vez em posições adjacentes.

## Análise Teórica
Este problema pode ser modelado utilizando Teoria dos Grafos:
- Os inteiros $1, \dots, N$ são os vértices de um grafo.
- Cada par de inteiros distintos $\{u, v\}$ representa uma aresta em um grafo completo $K_N$.
- A sequência desejada corresponde a um **Caminho Euleriano** no grafo $K_N$.

### Condições de Existência:
1. **Graus no $K_N$**: Cada vértice tem grau $d = N-1$.
2. **Caso $N$ par**: Se $N > 2$, todos os $N$ vértices possuem grau ímpar. Como um caminho Euleriano permite no máximo 2 vértices de grau ímpar, não existe solução para $N$ par e $N > 2$.
3. **Caso $N$ ímpar**: Todos os vértices possuem grau par ($N-1$ é par). Portanto, existe um **Circuito Euleriano**. Isso implica que a sequência deve começar e terminar no mesmo vértice.

## Estratégia de Solução
Para verificar se o prefixo $L$ e o sufixo $R$ podem ser completados:

1. **Validação Inicial**:
   - Se $N$ é par e $N > 2$, a resposta é `N`.
   - Se $N=2$, a sequência deve ser uma subsequência de $[1, 2]$ ou $[2, 1]$.
   - Para $N$ ímpar:
     - O comprimento total da sequência final deve ser $S = \frac{N(N-1)}{2} + 1$. Verificamos se $P + Q \le S$.
     - Verificamos se não há elementos adjacentes idênticos.
     - Verificamos se nenhuma aresta (par não ordenado) é repetida em $L$ e $R$.
     - Como deve ser um circuito, o primeiro elemento de $L$ deve ser igual ao último de $R$ ($L_1 = R_Q$).

2. **Conectividade das Arestas Restantes**:
   - Seja $G'$ o grafo formado pelas arestas de $K_N$ que não foram utilizadas em $L$ ou $R$.
   - Para que a sequência possa ser completada, deve existir um caminho em $G'$ que conecte o final de $L$ ($L_P$) ao início de $R$ ($R_1$), passando por todas as arestas restantes.
   - Pelo Teorema de Euler, isso é possível se:
     - Todas as arestas de $G'$ estiverem na mesma componente conectada.
     - Os pontos de extremidade $L_P$ e $R_1$ também pertencerem a essa componente (caso existam arestas em $G'$).

3. **Implementação**:
   - Construímos o grafo complementar $G'$ em $O(N^2)$.
   - Utilizamos a estrutura **Disjoint Set Union (DSU)** para gerenciar as componentes conectadas de $G'$.
   - Verificamos se há apenas uma componente que contém arestas e se ela engloba $L_P$ e $R_1$.

## Complexidade
- **Tempo**: $O(N^2 + P + Q)$, o que é eficiente para $N=1000$ e $P, Q=5 \cdot 10^5$.
- **Espaço**: $O(N^2)$ para armazenar a matriz de arestas usadas ou as adjacências de $G'$.
