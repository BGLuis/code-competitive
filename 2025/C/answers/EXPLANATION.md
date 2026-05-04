# Problema C - Ruas Limpas

## Descrição do Problema
O objetivo é contratar um subconjunto de faxineiros para limpar $S$ ruas em no máximo $K$ horas, minimizando o pagamento total. Cada faxineiro $i$ leva $H_i$ horas por rua e aceita um pagamento por rua $p_i \in [L_i, U_i]$. Uma restrição fundamental é que o pagamento por hora ($R = p_i/H_i$) deve ser o mesmo para todos os faxineiros contratados.

## Análise Matemática
Seja $R$ o pagamento por hora comum a todos os faxineiros contratados. O pagamento por rua do faxineiro $i$ será $p_i = R \cdot H_i$.
As restrições são:
1. $L_i \le R \cdot H_i \le U_i \Rightarrow R \in [\frac{L_i}{H_i}, \frac{U_i}{H_i}]$.
2. Cada faxineiro contratado $i$ limpa $s_i \ge 1$ ruas.
3. $\sum s_i = S$.
4. $s_i \cdot H_i \le K \Rightarrow s_i \le \lfloor K/H_i \rfloor$.

O custo total é $\sum s_i p_i = \sum s_i (R H_i) = R \sum s_i H_i$.
Para um valor fixo de $R$, queremos escolher os faxineiros $i$ cujo intervalo $I_i = [\frac{L_i}{H_i}, \frac{U_i}{H_i}]$ contenha $R$, e atribuir ruas $s_i$ para minimizar $R \sum s_i H_i$.
Como $R$ e $s_i$ são positivos, isso equivale a minimizar $\sum s_i H_i$.
A estratégia gulosa para minimizar $\sum s_i H_i$ é:
- Selecionar faxineiros com os menores valores de $H_i$.
- Atribuir o máximo de ruas possível ($M_i = \lfloor K/H_i \rfloor$) aos faxineiros mais eficientes até atingir o total $S$.

## Estratégia de Solução
O valor ótimo de $R$ sempre ocorrerá em um dos limites inferiores dos intervalos dos faxineiros disponíveis, ou seja, $R = L_k/H_k$ para algum $k$.
1. **Eventos**: Criamos eventos para o início ($L_i/H_i$) e o fim ($U_i/H_i$) de cada intervalo de validade de $R$ para cada faxineiro.
2. **Varredura (Sweep-line)**: Ordenamos os eventos e percorremos os possíveis valores de $R$.
3. **Estrutura de Dados**: Utilizamos uma **Árvore de Fenwick (BIT)** ou **Árvore de Segmentos** para manter o conjunto de faxineiros disponíveis para o valor atual de $R$, ordenados por $H_i$.
   - A árvore armazena a capacidade total de ruas ($\sum M_i$) e a soma dos custos de tempo ($\sum M_i H_i$).
4. **Consulta**: Para cada valor de $R$ candidato:
   - Verificamos se a capacidade total é $\ge S$.
   - Encontramos a soma mínima de $H$ para limpar $S$ ruas (usando busca binária/lifting sobre a árvore).
   - Calculamos o custo como a fração $\frac{L_k \cdot (\text{SomaH})}{H_k}$.
5. **Resultado**: Mantemos a menor fração encontrada e a imprimimos de forma irredutível (simplificada pelo MDC).

## Complexidade
- **Tempo**: $O(N \log N)$ para ordenação e processamento dos eventos com a árvore de Fenwick.
- **Espaço**: $O(N)$ para armazenar os faxineiros e a árvore.
