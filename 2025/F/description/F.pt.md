# Problema F - Fatoração Difusa

Encontrar a fatoração em números primos de números grandes é uma tarefa desafiadora. É tão difícil que a segurança de quase todo o nosso mundo digital, desde bancos online até mensagens privadas, é construída sobre essa dificuldade. Neste problema, você deve realizar essa fatoração, mas é permitido algum erro relativo na sua resposta.

Mais formalmente, você recebe um inteiro $X$, e você tem que fornecer a fatoração em primos de qualquer número $Y = p_1^{e_1} \cdot p_2^{e_2} \cdots p_k^{e_k}$ tal que:
* o erro relativo da fatoração não exceda $10^{-9}$ (ou seja, $|X - Y| / X \le 10^{-9}$), e
* cada fator primo $p_i$ de $Y$ não exceda $10^{18}$ (ou seja, $p_i \le 10^{18}$ para $i = 1, 2, \ldots, k$).

## Entrada

A entrada consiste em uma única linha que contém um inteiro $X$ ($2 \le X \le 10^{1000}$).

## Saída

A primeira linha deve conter um número inteiro positivo $k$ indicando o número de fatores primos diferentes na fatoração em primos de $Y = p_1^{e_1} \cdot p_2^{e_2} \cdots p_k^{e_k}$.

A $i$-ésima das próximas $k$ linhas deve conter dois inteiros positivos $p_i$ e $e_i$, representando que $p_i$ é um fator primo de $Y$ com multiplicidade $e_i$.

Pode-se provar que existe uma resposta válida sob as restrições dadas. Se houver várias soluções, imprima qualquer uma delas.

## Exemplos

| Exemplo de entrada 1 | Exemplo de saída 1 |
|:---------------------|:-------------------|
| `520`                | `3`<br>`5 1`<br>`2 3`<br>`13 1` |

| Exemplo de entrada 2 | Exemplo de saída 2 |
|:---------------------|:-------------------|
| `1073741825`         | `1`<br>`2 30`      |

**Explicação do exemplo 2:**
$X = 1073741825$, $Y = 2^{30} = 1073741824$, e o erro relativo é $|X - Y| / X = 1 / 1073741825 \le 10^{-9}$. Note que existem outras soluções válidas para este caso de teste.