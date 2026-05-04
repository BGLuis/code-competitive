# Problema K - Conquista dos Reis

## Descrição do Problema
O objetivo é maximizar a área do menor retângulo que contém $N$ reis em uma grade infinita, após realizar no máximo $K$ movimentos. Cada movimento permite deslocar um rei para qualquer uma das 8 células adjacentes (distância de Chebyshev).

## Análise Matemática
Sejam $H_0$ e $W_0$ a altura e a largura iniciais do retângulo envolvente (bounding box). A área é dada por $H \times W$.
Para expandir o retângulo, precisamos mover os reis para além dos limites atuais $[R_{min}, R_{max}]$ e $[C_{min}, C_{max}]$.
Como a distância de Chebyshev entre $(r_1, c_1)$ e $(r_2, c_2)$ é $\max(|r_1-r_2|, |c_1-c_2|)$, um rei pode aumentar simultaneamente a altura e a largura em $d$ unidades com apenas $d$ movimentos.

Existem duas estratégias fundamentais para maximizar a área $(H_0 + \Delta H)(W_0 + \Delta W)$:

1. **Expansão de Canto (Corner Move)**:
   Escolhemos um único rei e o movemos o máximo possível ($K$ passos) na direção de um dos quatro cantos (ex: diagonal superior direita). O ganho em cada dimensão depende da distância original do rei até a borda. Por exemplo, movendo o rei $i$ para o "Nordeste", a nova altura aumenta em $\max(0, K - (R_{max} - r_i))$ e a largura em $\max(0, K - (C_{max} - c_i))$.

2. **Expansão Ortogonal**:
   Dividimos os $K$ movimentos entre dois reis diferentes situados em bordas adjacentes (ex: um no topo e outro na direita). Se usarmos $x$ movimentos para subir e $K-x$ para ir à direita, a área será $(H_0 + x)(W_0 + K - x)$. Esta é uma função quadrática em $x$ que pode ser maximizada facilmente.

## Estratégia de Solução
1. **Caso base**: Se $N=1$, a área é sempre 1.
2. **Cálculo das Bordas**: Encontramos os limites iniciais $R_{min}, R_{max}, C_{min}, C_{max}$.
3. **Iteração por Reis**: Para cada um dos $N$ reis, testamos as 4 expansões de canto possíveis.
4. **Maximização da Parábola**: Testamos a divisão ideal de $K$ entre altura e largura, verificando o vértice da função $(H_0 + x)(W_0 + K - x)$.
5. **Resultado**: A resposta é o máximo entre todas as áreas calculadas.

## Complexidade
- **Tempo**: $O(N)$ para ler os dados e processar cada rei individualmente.
- **Espaço**: $O(N)$ para armazenar as coordenadas dos reis.
