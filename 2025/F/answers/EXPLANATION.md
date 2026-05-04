# Problema F - Fatoração Difusa

## Descrição do Problema
O objetivo é encontrar a fatoração em números primos de qualquer número $Y$ tal que o erro relativo entre $Y$ e um dado inteiro gigante $X$ ($1000$ dígitos) não exceda $10^{-9}$. Todos os fatores primos de $Y$ devem ser menores ou iguais a $10^{18}$.

## Análise Matemática
A restrição do erro relativo $|X - Y| / X \le 10^{-9}$ significa que os primeiros 9 ou 10 dígitos significativos de $Y$ devem coincidir com os de $X$. 
Seja $X$ escrito na forma:
$X = P \cdot 10^L + R$
onde $P$ são os primeiros $k$ dígitos de $X$, $L$ é a quantidade de dígitos restantes e $0 \le R < 10^L$.
Se escolhermos $Y = P \cdot 10^L$, o erro relativo será:
$\frac{|X - Y|}{X} = \frac{R}{P \cdot 10^L + R} < \frac{10^L}{P \cdot 10^L} = \frac{1}{P}$

Para garantir que o erro seja $\le 10^{-9}$, basta escolhermos um prefixo $P$ com pelo menos 10 dígitos ($P \ge 10^9$).

## Estratégia de Solução
1. **Escolha de $Y$**:
   - Se o número de dígitos de $X$ for pequeno ($\le 14$), usamos $Y = X$ e realizamos a fatoração exata.
   - Se $X$ for grande, tomamos os primeiros 14 dígitos de $X$ como $P$ e o restante como $L$ potências de 10. Assim, $Y = P \cdot 10^L$.
2. **Fatoração**:
   - Fatoramos o número $P$ (que possui no máximo 14 dígitos) usando o método de divisão por tentativa até $\sqrt{P} \approx 3,7 \cdot 10^6$. Este processo é extremamente rápido.
   - Como $10^L = 2^L \cdot 5^L$, adicionamos os fatores 2 e 5 com multiplicidade $L$ à fatoração de $P$.
3. **Verificação das Restrições**:
   - **Primalidade**: Os fatores primos de $P$ são menores que $10^{14}$, e os fatores de $10^L$ são 2 e 5. Todos são $\le 10^{18}$.
   - **Erro**: Com 14 dígitos no prefixo, o erro relativo é inferior a $10^{-13}$, superando a exigência de $10^{-9}$.

## Complexidade
- **Tempo**: $O(\sqrt{P} + \text{Dígitos}(X))$, onde $\sqrt{P} \le 10^7$. Isso permite processar números de 1000 dígitos em milissegundos.
- **Espaço**: $O(\text{Dígitos}(X))$ para armazenar o número de entrada.
