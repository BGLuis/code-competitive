# Problem F - Fuzzy Factorization

Finding the prime factorization of big numbers is a challenging task. It is so difficult that the security of almost our entire digital world, from online banking to private messages, is built upon how hard it is. In this problem, you are asked to perform such a factorization, but some relative error is allowed in your answer.

More formally, you are given an integer $X$, and you have to provide the prime factorization of any number $Y = p_1^{e_1} \cdot p_2^{e_2} \cdots p_k^{e_k}$ such that:
* the relative error of the factorization does not exceed $10^{-9}$ (that is, $|X - Y| / X \le 10^{-9}$), and
* each prime factor $p_i$ of $Y$ does not exceed $10^{18}$ (that is, $p_i \le 10^{18}$ for $i = 1, 2, \ldots, k$).

## Input

The input consists of a single line that contains an integer $X$ ($2 \le X \le 10^{1000}$).

## Output

The first line must contain a positive integer $k$ indicating the number of different prime factors in the prime factorization of $Y = p_1^{e_1} \cdot p_2^{e_2} \cdots p_k^{e_k}$.

The $i$-th of the next $k$ lines must contain the two positive integers $p_i$ and $e_i$, representing that $p_i$ is a prime factor of $Y$ with multiplicity $e_i$.

It can be proven that a valid answer exists under the given constraints. If there are multiple solutions, output any of them.

## Samples

| Sample input 1 | Sample output 1 |
|:---------------|:----------------|
| `520`          | `3`<br>`5 1`<br>`2 3`<br>`13 1` |

| Sample input 2 | Sample output 2 |
|:---------------|:----------------|
| `1073741825`   | `1`<br>`2 30`   |

**Explanation of sample 2:**
$X = 1073741825$, $Y = 2^{30} = 1073741824$, and the relative error is $|X - Y| / X = 1 / 1073741825 \le 10^{-9}$. Note that there are other valid solutions for this test case.