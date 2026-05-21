# Problem B - 456

There are three dice, each with six faces.

The $j$-th face of the $i$-th die has $A_{i,j}$ written on it.

For each die, each face comes up with probability $\frac{1}{6}$.

When these dice are rolled simultaneously, find the probability that the values $4,5,6$ each appear exactly once.

## Constraints

- $A_{i,j}$ is an integer between $1$ and $6$, inclusive.

## Input / Output

### Input

The input is given from Standard Input in the following format:

$A_{1,1}$ $A_{1,2}$ $A_{1,3}$ $A_{1,4}$ $A_{1,5}$ $A_{1,6}$
$A_{2,1}$ $A_{2,2}$ $A_{2,3}$ $A_{2,4}$ $A_{2,5}$ $A_{2,6}$
$A_{3,1}$ $A_{3,2}$ $A_{3,3}$ $A_{3,4}$ $A_{3,5}$ $A_{3,6}$

### Output

Output the answer.
The answer is considered correct if the absolute or relative error from the true answer is at most $10^{-6}$.

## Samples

### Sample Input 1

```
1 2 3 4 5 6
1 2 3 4 5 6
1 2 3 4 5 6
```

### Sample Output 1

```
0.0277777778
```

**Explanation of sample 1:**
The desired probability is $\frac{1}{36}$. An output such as `0.027778` is also accepted.

### Sample Input 2

```
4 5 6 4 5 6
4 4 5 5 6 6
6 5 4 4 5 6
```

### Sample Output 2

```
0.2222222222
```

**Explanation of sample 2:**
The desired probability is $\frac{2}{9}$.

