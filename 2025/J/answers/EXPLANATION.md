# Problema J - Multidão Julgadora

## Descrição do Problema
O objetivo é calcular a pontuação emocional de uma apresentação de comédia baseada nos sons captados em uma string $S$. Três padrões específicos alteram a pontuação:
- Cada ocorrência de "ha" aumenta a pontuação em 1.
- Cada ocorrência de "boooo" diminui a pontuação em 1.
- Cada ocorrência de "bravo" aumenta a pontuação em 3.

## Análise Matemática
O problema pede a contagem total de ocorrências de padrões de texto em uma string. De acordo com os exemplos e a descrição, cada vez que um padrão aparece em um índice diferente da string, ele deve ser contabilizado, independentemente de sobreposições (embora os padrões específicos dificultem sobreposições entre si, exceto no caso de "ha" repetidos como "hahaha").

## Estratégia de Solução
Dada a restrição de tamanho da string ($|S| \le 1000$), uma abordagem direta de varredura linear é eficiente o suficiente.
1. Percorremos a string índice por índice.
2. Em cada posição $i$, verificamos se a substring que começa em $i$ corresponde a "ha", "boooo" ou "bravo".
3. Somamos os valores correspondentes ao placar total.
4. Funções nativas como `startsWith` (Java/Kotlin/Python) ou `compare`/`strncmp` (C++/C) facilitam essa verificação garantindo que não ultrapassemos os limites da string.

## Complexidade
- **Tempo**: $O(|S|)$, pois realizamos um número constante de comparações para cada caractere da string. Com $|S| = 1000$, o processamento é imediato.
- **Espaço**: $O(|S|)$ para armazenar a string de entrada.
