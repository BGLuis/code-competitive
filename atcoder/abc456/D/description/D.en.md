# Problem D - Not Adjacent 2

You are given a string $S$ consisting of `a`, `b`, `c`.

Find the number of non-empty subsequences of $S$ in which no two adjacent characters are the same, modulo $998244353$.

Two subsequences are considered distinct if they are taken from different positions, even if they are identical as strings.

What is a subsequence?
A subsequence of $S$ is a string obtained by removing zero or more characters from $S$ and concatenating the remaining characters in their original order.  
For example, `ab`, `ac` are subsequences of `abc`, but `ca`, `bb` are not subsequences of `abc`.

## Constraints

- $S$ is a string of length between $1$ and $3 \times 10^5$, inclusive, consisting of `a`, `b`, `c`.

## Input / Output

### Input

The input is given from Standard Input in the following format:

$S$

### Output

Output the answer.

## Samples

### Sample Input 1

```
abbc
```

### Sample Output 1

```
11
```

**Explanation of sample 1:**
The subsequences in which no two adjacent characters are the same are the following $11$:

- `a` (the $1$st character of $S$)
- `b` (the $2$nd character of $S$)
- `b` (the $3$rd character of $S$)
- `c` (the $4$th character of $S$)
- `ab` (the $1$st, $2$nd characters of $S$)
- `ab` (the $1$st, $3$rd characters of $S$)
- `ac` (the $1$st, $4$th characters of $S$)
- `bc` (the $2$nd, $4$th characters of $S$)
- `bc` (the $3$rd, $4$th characters of $S$)
- `abc` (the $1$st, $2$nd, $4$th characters of $S$)
- `abc` (the $1$st, $3$rd, $4$th characters of $S$)

Note that, as with the $2$nd and $3$rd entries, two subsequences are considered distinct if they are taken from different positions, even if they are identical as strings.

### Sample Input 2

```
cabcabcbcaccacbcbcaabacbacaabccacbccbcacbacbacabcacabcaccaaaaabababcbabacaccabbcacbcbcbcababcbcbabca
```

### Sample Output 2

```
378217423
```

**Explanation of sample 2:**
Output the count modulo $998244353$.

