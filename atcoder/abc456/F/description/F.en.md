# Problem F - Plan Holidays

Takahashi is trying to decide his schedule for $N$ days. Initially, none of the days are holidays.

He can repeat either of the following operations any number of times:

- Choose an integer $i$ between $1$ and $N$, inclusive, and make day $i$ a holiday. This operation costs $A_i$.
- Choose an integer $i$ between $2$ and $N-1$, inclusive, such that both day $i-1$ and day $i+1$ are already holidays, and make day $i$ a holiday. This operation is free.

Find the minimum total cost required to create a consecutive block of $K$ or more holidays.

$T$ test cases are given; solve each of them.

## Constraints

- $1 \leq T \leq 2 \times 10^5$
- $1 \leq K \leq N \leq 2 \times 10^5$
- $1 \leq A_i \leq 10^9$
- All input values are integers.
- The sum of $N$ over all test cases is at most $2\times 10^5$.

## Input / Output

### Input

The input is given from Standard Input in the following format:

$T$
$\mathrm{case}_1$
$\mathrm{case}_2$
$\vdots$
$\mathrm{case}_T$

Here, $\mathrm{case}_i$ denotes the input for the $i$-th test case. Each test case is given in the following format:

$N$ $K$
$A_1$ $A_2$ $\dots$ $A_N$

### Output

Output $T$ lines. The $i$-th line should contain the answer for the $i$-th test case.

## Samples

### Sample Input 1

```
3
5 2
3 1 4 1 5
6 4
24 3 22 39 4 29
15 7
220651272 302798780 874479994 657822311 613294668 479624013 241168404 610547619 762548286 256160531 823041612 951553052 226556081 649525901 153805947
```

### Sample Output 1

```
2
29
1902064780
```

**Explanation of sample 1:**
For the first test case, a consecutive block of at least two holidays can be created by performing operations as follows:

- Make day $2$ a holiday using the first type of operation. This costs $1$.
- Make day $4$ a holiday using the first type of operation. This costs $1$.
- Make day $3$ a holiday using the second type of operation. This is free.

The total cost of this sequence of operations is $2$. It is impossible to create a consecutive block of two or more holidays with a cost less than $2$, so output $2$.

