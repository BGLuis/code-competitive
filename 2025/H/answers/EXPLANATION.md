# Problema H - Horizontes Mais Difíceis

## Descrição do Problema
Lola deseja dividir sua lista de $N$ tarefas em grupos consecutivos (dias), de modo que o maior nível de dificuldade em cada dia seja estritamente superior ao do dia anterior. O objetivo é maximizar o número total de dias.

## Análise Matemática
Seja $M_j$ a dificuldade máxima do grupo de tarefas correspondente ao dia $j$. A restrição é $M_1 < M_2 < \dots < M_K$.
Seja $i_j$ o índice da tarefa que encerra o dia $j$. Temos:
$M_j = \max(D_{i_{j-1}+1}, \dots, D_{i_j})$

Podemos provar que $M_j$ deve ser igual ao valor do máximo de prefixo no índice $i_j$:
1. Pela definição de máximo, $M_j \ge D_k$ para todo $k$ no grupo $j$.
2. Por indução, se $M_{j-1}$ era o máximo de prefixo até $i_{j-1}$, e $M_j > M_{j-1}$, então $M_j$ deve ser o novo máximo de prefixo atingido em algum ponto entre $i_{j-1}+1$ e $i_j$.
3. Como queremos maximizar $K$, devemos criar um novo grupo toda vez que encontrarmos uma tarefa que estabeleça um novo recorde de dificuldade na sequência (um novo máximo de prefixo). Cada uma dessas tarefas pode atuar como o $M_j$ de um novo dia.

## Estratégia de Solução
O problema resume-se a contar quantos elementos na sequência original são maiores que todos os elementos que os precedem.
1. Percorremos a lista de dificuldades mantendo o valor do maior elemento visto até o momento (`curr_max`).
2. Toda vez que encontramos um elemento $D_i > curr\_max$, incrementamos o contador de dias e atualizamos `curr_max`.

## Complexidade
- **Tempo**: $O(N)$, uma única passagem pela lista.
- **Espaço**: $O(1)$ se lermos a entrada em fluxo, ou $O(N)$ para armazenar as tarefas.
