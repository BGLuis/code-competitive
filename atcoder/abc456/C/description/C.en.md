# Problem C - Not Adjacent

You are given a string $S$ consisting of `a`, `b`, `c`.

Find the number of non-empty substrings of $S$ in which no two adjacent characters are the same, modulo $998244353$.

Two substrings are considered distinct if they are taken from different positions, even if they are identical as strings.

What is a substring?
A substring of $S$ is a string obtained by deleting zero or more characters from the beginning and zero or more characters from the end of $S$.  
For example, `ab` is a substring of `abc`, but `ac` is not a substring of `abc`.

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
6
```

**Explanation of sample 1:**
The substrings in which no two adjacent characters are the same are the following six:

- `a` (from the $1$st to the $1$st character of $S$)
- `b` (from the $2$nd to the $2$nd character of $S$)
- `b` (from the $3$rd to the $3$rd character of $S$)
- `c` (from the $4$th to the $4$th character of $S$)
- `ab` (from the $1$st to the $2$nd character of $S$)
- `bc` (from the $3$rd to the $4$th character of $S$)

Note that, as with the $2$nd and $3$rd entries, two substrings are considered distinct if they are taken from different positions, even if they are identical as strings.

### Sample Input 2

```
cabcabcbcaccacbcbcaabacbacaabccacbccbcacbacbacabcacabcaccaaaaabababcbabacaccabbcacbcbcbcababcbcbabca
```

### Sample Output 2

```
760
```

