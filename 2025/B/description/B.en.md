# Problem B - Balanced Balloons

A group of students is starting a competitive programming club, and they expect that $N$ people will join, one by one. When each person joins, they must bring between 1 and $K$ balloons.

The founders want to make sure things stay fair and decided to impose the following fairness rule: after each person joins, it must be possible to evenly redistribute all balloons collected so far among the current members. In other words, after each step, the total number of balloons so far must be divisible by the current number of people.

Being competitive programmers, they realized that this makes for a nice counting problem, and decided to challenge you. How many sequences of $N$ balloon contributions (each contribution between 1 and $K$) satisfy the fairness rule at every step?

Two sequences are considered different if they differ at any position.

## Input

The input consists of a single line that contains two integers $N$ ($1 \le N \le 10^9$) and $K$ ($1 \le K \le 2000$), indicating respectively the number of members and the maximum number of balloons each member can bring.

## Output

Output a single line with an integer indicating the number of balloon contribution sequences that satisfy the fairness rule. Because this number can be very large, output the remainder of dividing it by $998244353$.

## Samples

| Sample input 1 | Sample output 1 |
|:---------------|:----------------|
| `3 3`          | `5`             |

**Explanation of sample 1:**
The 5 sequences of balloon contributions respecting the fairness rule are:
$[1, 1, 1]$,
$[1, 3, 2]$,
$[2, 2, 2]$,
$[3, 1, 2]$,
$[3, 3, 3]$.

| Sample input 2 | Sample output 2 |
|:---------------|:----------------|
| `4 1`          | `1`             |

**Explanation of sample 2:**
Everyone must contribute with a single balloon so the only possible sequence is $[1, 1, 1, 1]$.