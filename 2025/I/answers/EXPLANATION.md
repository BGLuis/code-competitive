# Problema I - Rota de Infiltração

## Descrição do Problema
O objetivo é encontrar uma rota de elevadores do andar 1 ao andar $2N$ em um edifício de $2N$ andares. Existem $N$ sensores, cada um monitorando um par de andares $\{i, i+N\}$. A restrição é que a rota não pode visitar dois andares monitorados pelo mesmo sensor (ou seja, no máximo um andar de cada par).

## Análise Matemática
Este problema pode ser modelado como a busca de um caminho em um Grafo Acíclico Dirigido (DAG) onde os vértices possuem cores (sensores) e o caminho deve conter apenas cores distintas.
Em um grafo geral, este problema é NP-difícil (Colorful Path), mas a estrutura de DAG e os limites de $N$ e $M$ permitem uma abordagem de busca eficiente.

Como cada par de andares $\{i, i+N\}$ compartilha o mesmo sensor, e os códigos de elevador sempre sobem ($S < T$), um conflito ocorre se visitarmos o andar $i$ e, posteriormente, o andar $i+N$.

## Estratégia de Solução
Utilizamos uma busca em profundidade (**DFS**) combinada com **Memoização de Bitsets** para explorar os caminhos possíveis.

1. **Estado da Busca**: O estado é definido pelo par `(andar atual, máscara de sensores usados)`. A máscara é um bitset de $N$ bits onde o $i$-ésimo bit indica se o sensor $i$ já foi ativado por algum andar na rota atual.
2. **Poda por Máscaras de Falha**:
   - Para cada andar $u$, mantemos uma lista de máscaras de sensores que levaram a uma falha (não alcançaram o andar $2N$).
   - Se ao chegar no andar $u$, a máscara atual de sensores for um superconjunto de alguma máscara que já falhou para esse mesmo andar, podemos interromper a busca imediatamente (poda). Isso é válido porque se não foi possível chegar ao destino com menos restrições, certamente não será possível com mais restrições.
3. **Representação de Bitset**:
   - No Python, utilizamos inteiros de precisão arbitrária para representar a máscara.
   - No C/C++, utilizamos `std::bitset<505>` ou um array de `uint64_t`.

## Complexidade
- **Tempo**: Embora o espaço de estados teórico seja exponencial ($2N \cdot 2^N$), a natureza esparsa do grafo ($M=1000$) e a eficácia da poda por subconjuntos de máscaras tornam a execução extremamente rápida para as instâncias fornecidas.
- **Espaço**: $O(2N + M)$ para o grafo e proporcional ao número de estados visitados para a memoização.
