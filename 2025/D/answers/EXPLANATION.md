# Problema D - Exibindo Decimais

## Descrição do Problema
O objetivo é calcular o número esperado de dígitos necessários para exibir a taxa de conversão $A_i / A_j$ entre duas moedas escolhidas aleatoriamente entre as $N$ moedas disponíveis. Para dízimas periódicas, exibimos apenas o primeiro período (sem contar separadores ou barras).

## Regras de Contagem de Dígitos
Para uma fração $x/y = A_i/A_j$:
1. **Parte Inteira ($D_{int}$)**:
   - Se $x/y \ge 1$, o número de dígitos é $\lfloor \log_{10}(x/y) \rfloor + 1$.
   - Se $x/y < 1$, o número de dígitos é 1 (o "0" de "0.xxx").
2. **Parte Decimal**:
   - Seja $a/b$ a fração irredutível. Decompomos $b = 2^n \cdot 5^m \cdot b'$, onde $\gcd(b', 10) = 1$.
   - **Parte Não-Periódica ($D_{non}$)**: O comprimento é $\max(n, m)$.
   - **Período ($D_{rep}$)**: Se $b' > 1$, o comprimento do período é a ordem multiplicativa de 10 módulo $b'$ ($\text{ord}_{b'}(10)$). Se $b'=1$, é 0.

O total de dígitos é dado por $D_{int} + D_{non} + D_{rep}$.

## Estratégia de Solução
Queremos calcular $\frac{1}{N^2} \sum_{i,j} (D_{int}(i,j) + D_{non}(i,j) + D_{rep}(i,j))$.

### 1. Soma da Parte Inteira ($D_{int}$)
Utilizamos a frequência dos valores $A_i$ (vetor $C$).
Observamos que $\text{Digits}(v/u) = 1 + [v \ge 10u] + [v \ge 100u] + \dots$
A soma total para a parte inteira é:
$N^2 + \sum_u C[u] \sum_{k=1}^5 \text{Count}(v \ge u \cdot 10^k)$.
Esta parte é calculada em $O(V)$ usando somas de prefixos de $C$.

### 2. Soma da Parte Decimal ($D_{non} + D_{rep}$)
Seja $f(b) = D_{non}(b) + D_{rep}(b)$ a soma dos comprimentos decimal e periódico do denominador reduzido $b = A_j / \gcd(A_i, A_j)$.
A soma desejada é $\sum_u \sum_v C[u] C[v] f(u / \gcd(u, v))$.
Utilizando a propriedade do MDC e inversão de Mobius, a soma pode ser rearranjada como:
$Sum = \sum_d \mu(d) \sum_k f(dk) \sum_r C[dkr] T(dr)$
Onde $T(X) = \sum_{j \cdot X \le V} C[jX]$.
Esta expressão permite o cálculo em $O(V (\ln V)^2)$, percorrendo triplas $(d, k, r)$ tais que $dkr \le V$. Para $V=10^5$, isso resulta em aproximadamente $1.3 \cdot 10^7$ iterações.

## Implementação
- **Pré-processamento**: Crivo de Eratóstenes para $\mu$, $\phi$ e o menor fator primo (SPF). Cálculo de $f(b)$ usando fatoração de $\phi(b)$ para encontrar a ordem de 10 mod $b'$.
- **Complexidade**:
  - Tempo: $O(V (\ln V)^2 + N)$.
  - Espaço: $O(V)$.
- O resultado é calculado utilizando aritmética modular e inverso multiplicativo.
