# Problem E - Endless Holidays

The Kingdom of AtCoder has $N$ cities, called city $1,2,\dots,N$.
There are $M$ bidirectional roads connecting pairs of cities, where the $i$-th road connects cities $U_i$ and $V_i$.
Any pair of cities can be reached from each other by traversing some roads.

In the Kingdom of AtCoder, a week consists of $W$ days.
A week proceeds through days $1,2,\dots,W$, and the day after day $W$ is day $1$.

Each city has certain days of the week that are holidays.
The holiday information for city $i$ is given as a string $S_i$ of length $W$: if the $j$-th character of $S_i$ is `o`, day $j$ is a holiday; if it is `x`, day $j$ is a weekday.

Takahashi chooses a city he likes and visits it at noon on day $1$.
Each night thereafter, he repeatedly chooses to either stay in his current city or move to a city directly connected by a road.
Output `Yes` if it is possible for him to keep moving so that the city he is in at noon every day is a holiday, and `No` otherwise.

$T$ test cases are given; solve each of them.

## Constraints

- $1 \leq T \leq 10^5$
- $1 \leq N \leq 10^5$
- $N-1 \leq M \leq 10^5$
- $1 \leq U_i \lt V_i \leq N$
- Any pair of cities can be reached from each other by traversing some roads.
- $2 \leq W \leq 10$
- $T,N,M,U_i,V_i,W$ are integers.
- $S_i$ is a string of length $W$ consisting of `o`, `x`.
- The sum of $N$ over all test cases is at most $10^5$.
- The sum of $M$ over all test cases is at most $10^5$.

## Input / Output

### Input

The input is given from Standard Input in the following format:

$T$
$\mathrm{case}_1$
$\mathrm{case}_2$
$\vdots$
$\mathrm{case}_T$

Here, $\mathrm{case}_i$ denotes the input for the $i$-th test case. Each test case is given in the following format:

$N$ $M$
$U_1$ $V_1$
$U_2$ $V_2$
$\vdots$
$U_M$ $V_M$
$W$
$S_1$
$S_2$
$\vdots$
$S_N$

### Output

Output $T$ lines. The $i$-th line should contain the answer for the $i$-th test case.

## Samples

### Sample Input 1

```
3
4 4
1 2
1 4
2 4
2 3
3
xxo
xox
oxo
oxx
1 0
4
oooo
5 5
1 4
2 3
4 5
3 4
2 5
7
oxxxxxx
xxoxxxo
xxxoxox
xoxxoxx
oxxxoxx
```

### Sample Output 1

```
Yes
Yes
No
```

**Explanation of sample 1:**
For the first test case, for example, the condition can be satisfied by repeatedly moving along cities $4 \to 2 \to 1 \to 4 \to 2 \to 1 \to \cdots$.
Alternatively, the condition can also be satisfied by repeatedly moving along cities $3 \to 2 \to 3 \to 3 \to 2 \to 3 \to \cdots$.

For the second test case, the condition can be satisfied by staying in city $1$ indefinitely.

For the third test case, it is impossible to keep moving while satisfying the condition.

