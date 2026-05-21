# Problem G - Count Holidays

Takahashi is creating a work schedule for $N$ days by designating each day as a workday or a holiday.

You are given a string $S$ of length $N$ representing the constraints on work days.
If the $i$-th character of $S$ is `x`, day $i$ must be a workday. If it is `.`, day $i$ can be either a workday or a holiday.

There are $2^q$ valid work schedules satisfying the constraints, where $q$ is the number of `.` characters in $S$.
For each $k=1,2,\dots,N$, solve the following problem:

Among the $2^q$ valid work schedules satisfying the constraints, find the number, modulo $998244353$, of schedules in which the longest consecutive block of holidays is exactly $k$ days.

## Constraints

- $N$ is an integer between $1$ and $2 \times 10^5$, inclusive.
- $S$ is a string of length $N$ consisting of `.`, `x`.

## Input / Output

### Input

The input is given from Standard Input in the following format:

$N$
$S$

### Output

Output $N$ lines. The $i$-th line should contain the answer for $k=i$.

## Samples

### Sample Input 1

```
5
.x...
```

### Sample Output 1

```
9
4
2
0
0
```

**Explanation of sample 1:**
Denoting holidays as `o`, the valid work schedules are as follows:

- $k=1$: `oxxxx`, `oxoxx`, `oxoxo`, `oxxox`, `oxxxo`, `xxoxx`, `xxoxo`, `xxxox`, `xxxxo`
- $k=2$: `oxoox`, `oxxoo`, `xxoox`, `xxxoo`
- $k=3$: `oxooo`, `xxooo`

### Sample Input 2

```
7
.......
```

### Sample Output 2

```
33
47
27
12
5
2
1
```

### Sample Input 3

```
20
.....x...x..........
```

### Sample Output 3

```
9359
75312
94664
46840
23680
7168
3072
1280
512
256
0
0
0
0
0
0
0
0
0
0
```

