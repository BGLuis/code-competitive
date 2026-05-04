# Problem H - Harder Horizons

Lola keeps a long to-do list of tasks she needs to finish. Each task has a difficulty number (the bigger the number, the harder the task), and the tasks must be completed in the order they appear.

The list keeps growing because items are rarely removed. Tired of that, Lola decided to take action: she wants to split her list into several consecutive groups of tasks, where each group represents the work she will do in one day.

To stay motivated, Lola wants to feel that each day is a step forward. So, starting from the second day, the hardest task of that day must be strictly harder than the hardest task from the previous day.

Lola also does not want to do too much in one day, so she wants to spread the work over as many days as possible.

What is the maximum number of days she can split the tasks into under these rules?

## Input

The first line contains an integer $N$ ($1 \le N \le 10^5$) indicating the number of tasks.

The second line contains $N$ integers $D_1, D_2, \dots, D_N$ ($1 \le D_i \le 10^5$ for $i = 1, 2, \dots, N$), representing the difficulties of the tasks in the order they appear in the list.

## Output

Output a single line with an integer indicating the maximum number of days Lola can split the tasks into, such that the hardest task in each day is strictly harder than the hardest task in the previous day.

## Samples

| Sample input 1 | Sample output 1 |
|:---------------|:----------------|
| `5`<br>`1 2 1 3 4` | `4`             |

**Explanation of sample 1:**
One optimal way to split the tasks in 4 days is: `1 | 2 1 | 3 | 4` with strictly increasing difficulties of the hardest tasks: $1 < 2 < 3 < 4$.

| Sample input 2 | Sample output 2 |
|:---------------|:----------------|
| `6`<br>`2 1 4 2 3 5` | `3`             |

**Explanation of sample 2:**
Here an optimal way to split the list in 3 days is: `2 1 | 4 2 | 3 5` with $2 < 4 < 5$.