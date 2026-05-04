# Problema L - Criaturas Solitárias

## Descrição do Problema
O objetivo é contar quantos pares de criaturas, cujas trajetórias são retas $y = M_i x + C_i$, possuem um ponto de encontro (interseção) situado estritamente dentro de um parquinho definido pela região $y > Ax^2 + B$.

## Análise Matemática
Para que o ponto de encontro $(x, y)$ das retas $i$ e $j$ esteja no parquinho, ele deve satisfazer a inequação $y > Ax^2 + B$. Como o ponto está sobre as retas, podemos substituir $y$ pela equação de uma das retas:
$M_i x + C_i > Ax^2 + B \Rightarrow Ax^2 - M_i x + (B - C_i) < 0$

Como $A > 0$, a expressão $Q_i(x) = Ax^2 - M_i x + (B - C_i)$ define uma parábola voltada para cima. A condição $Q_i(x) < 0$ é satisfeita se e somente se $x$ estiver entre as duas raízes reais de $Q_i(x) = 0$.
Seja $I_i = (L_i, R_i)$ o intervalo de $x$ onde a reta $i$ está acima da parábola.
O ponto de encontro de duas retas $i$ e $j$ está no parquinho se o seu valor de $x$ pertencer a $I_i \cap I_j$.

**Condição de Entrelaçamento**:
Dois intervalos $(L_i, R_i)$ e $(L_j, R_j)$ cujos pontos de extremidade estão sobre a fronteira do parquinho (a parábola) resultam em um ponto de encontro interno se e somente se os intervalos se **entrelaçam** (ex: $L_i < L_j < R_i < R_j$).
- Se forem disjuntos, não há interseção em $x$.
- Se um estiver contido no outro, a geometria das retas garante que a interseção ocorre fora da região superior à parábola.

## Estratégia de Solução
1. **Cálculo de Intervalos**: Para cada reta, resolvemos a equação quadrática para encontrar $L_i$ e $R_i$. Ignoramos retas que não cruzam a parábola.
2. **Contagem de Entrelaçamentos**:
   - Ordenamos os intervalos pelo ponto inicial $L_i$.
   - Utilizamos uma técnica de **Varredura (Sweep-line)** combinada com uma **Árvore de Fenwick (BIT)**.
   - Ao processar o intervalo $i$, buscamos na árvore quantos intervalos $j < i$ (já iniciados) terminam entre $L_i$ e $R_i$. Isso identifica precisamente os pares que satisfazem $L_j < L_i < R_j < R_i$.
   - Inserimos o ponto $R_i$ na árvore para processamentos futuros.
3. **Precisão**: Utilizamos `double` ou `long double` e uma pequena margem de erro ($\epsilon$) para lidar com as raízes.

## Complexidade
- **Tempo**: $O(N \log N)$ devido à ordenação e às operações na árvore de Fenwick.
- **Espaço**: $O(N)$ para armazenar os intervalos e a estrutura de dados.
