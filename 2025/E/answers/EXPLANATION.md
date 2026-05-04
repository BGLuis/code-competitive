# Problema E - Rações de Emergência

## Descrição do Problema
O objetivo é determinar o número mínimo de dias para esvaziar um conjunto de caixas de ração. A cada dia, podemos realizar uma de duas ações:
1. Escolher uma caixa e esvaziá-la completamente.
2. Remover uma ração de cada caixa que ainda contém algo.

O suprimento de caixas muda dinamicamente através de adições e remoções. Devemos calcular o tempo mínimo após cada operação.

## Análise Matemática
Seja $a_1, a_2, \dots, a_n$ o número de rações em cada caixa. Se decidirmos usar a estratégia de remover uma ração de todas as caixas (ação tipo 2) por exatamente $d$ dias, as caixas que originalmente tinham $a_i \le d$ rações serão esvaziadas. As caixas com $a_i > d$ ainda terão rações e devem ser esvaziadas individualmente (ação tipo 1).

O tempo total de dias será dado por:
$f(d) = d + \text{count}(a_i > d)$

Queremos minimizar $f(d)$ para $d \ge 0$. Como a função $\text{count}(a_i > d)$ só muda nos valores $a_i$ existentes, o valor ótimo de $d$ será $0$ ou algum $a_i$.
Definindo $\text{rank}(d)$ como a quantidade de caixas com no máximo $d$ rações, temos:
$f(d) = d + n - \text{rank}(d) = n + (d - \text{rank}(d))$

## Estratégia de Solução
Devemos manter dinamicamente o valor de $n + \min(0, \min_{i} (a_i - \text{rank}(a_i)))$.

1. **Discretização**: Coletamos todos os valores absolutos de $X_i$ presentes nas consultas, ordenamos e removemos duplicatas para mapear os valores para índices pequenos ($1 \dots m$).
2. **Árvore de Segmentos**:
   - Utilizamos uma árvore de segmentos com **Lazy Propagation** para suportar atualizações de intervalo e consultas de mínimo.
   - Cada folha $k$ representa o valor $v_k$ das rações. Se houver pelo menos uma caixa desse tamanho, a folha armazena $v_k - \text{rank}(v_k)$. Caso contrário, armazena um valor infinito ($\infty$).
   - **Adição de caixa ($v_k$)**:
     - Incrementamos o número total de caixas $n$.
     - Subtraímos 1 no intervalo $[k, m]$ da árvore (pois o rank de todos os valores maiores ou iguais a $v_k$ aumentou).
     - Se for a primeira caixa desse tamanho, ativamos a folha $k$ com seu valor atual.
   - **Remoção de caixa ($v_k$)**:
     - Realizamos o processo inverso da adição.
3. **Cálculo da Resposta**: O valor mínimo é dado por $n + \min(0, \text{tree\_min})$. O termo $0$ representa o caso onde $d=0$ (esvaziar todas as caixas individualmente).

## Complexidade
- **Tempo**: $O(Q \log Q)$, onde $Q$ é o número de operações, devido à ordenação e às $Q$ operações na árvore de segmentos.
- **Espaço**: $O(Q)$ para a estrutura da árvore e mapeamento de valores.
