# Problema B - Balões Equilibrados

## Descrição do Problema
O problema pede para contar quantas sequências de $N$ contribuições de balões $[b_1, b_2, \dots, b_N]$ (onde $1 \le b_i \le K$) satisfazem a condição de que, após cada passo $i$, a soma total dos balões arrecadados $S_i = \sum_{j=1}^i b_j$ seja divisível por $i$.

## Análise Matemática
A condição de justiça no passo $i$ é:
$S_i \equiv 0 \pmod i$

Isso implica que $S_{i-1} + b_i \equiv 0 \pmod i$.
Seja $q_{i-1} = \frac{S_{i-1}}{i-1}$ o número de balões por pessoa no passo anterior.
Temos $S_{i-1} = q_{i-1}(i-1)$.
A condição torna-se:
$q_{i-1}(i-1) + b_i \equiv 0 \pmod i$
$q_{i-1}(-1) + b_i \equiv 0 \pmod i$
$b_i \equiv q_{i-1} \pmod i$

Portanto, em cada passo $i$, devemos escolher $b_i \in [1, K]$ tal que $b_i$ deixe o mesmo resto que $q_{i-1}$ quando dividido por $i$.
Seja $b_i = q_{i-1} + m \cdot i$. Então:
$q_i = \frac{S_i}{i} = \frac{q_{i-1}(i-1) + q_{i-1} + m \cdot i}{i} = q_{i-1} + m$.

Como $1 \le b_i \le K$ e $1 \le q_{i-1} \le K$, o número de escolhas para $m$ (e consequentemente para $q_i$) é limitado.
Notavelmente, se $i > K$, o único valor possível para $m$ é $0$, pois:
- Se $m \ge 1$, $b_i \ge q_{i-1} + i > K$.
- Se $m \le -1$, $b_i \le q_{i-1} - i < 0$.
Assim, para $i > K$, há apenas uma escolha ($b_i = q_{i-1}$) e o valor de $q_i$ não muda.

## Algoritmo (Programação Dinâmica)
Utilizamos DP para contar as sequências válidas:
- **Estado**: $dp[i][q]$ é o número de sequências de comprimento $i$ tais que $S_i/i = q$.
- **Transição**:
  Para cada $q_{old}$ no passo $i-1$, os valores possíveis de $q_{new}$ no passo $i$ são:
  $q_{new} \in [q_{old} - \lfloor \frac{q_{old}-1}{i} \rfloor, q_{old} + \lfloor \frac{K-q_{old}}{i} \rfloor]$
- **Otimização**: Como a transição é feita sobre um intervalo, utilizamos **Somas de Prefixos** (Difference Array) para realizar as atualizações em $O(1)$ por estado, resultando em uma complexidade total de $O(K^2)$.

## Complexidade
- **Tempo**: $O(K^2)$, pois iteramos $i$ até no máximo $K$ e para cada $i$ processamos $K$ estados.
- **Espaço**: $O(K)$ para armazenar os valores da DP do passo anterior.
