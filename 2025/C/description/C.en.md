# Problem C - Clean Streets

One more year of Virada Cultural in São Paulo. The city is full of people, and the streets are full of life. People from all over the city go to the streets to enjoy a night of cultural events and a lot of music.

Oswaldo, an employee at the city hall, was assigned the task of hiring cleaners to clean the $S$ streets of São Paulo, in no more than $K$ hours after this big event. He was given by his boss a list of $N$ cleaners he could hire, numbered from 1 to $N$, along with information related to how fast they work and the payment they accept. According to the list, cleaner $i$ can clean any street in $H_i$ hours, and accepts as payment anything between $L_i$ and $U_i$ for cleaning each street.

Oswaldo must hire a subset $C$ of the cleaners that are in the list. For each hired cleaner $i \in C$ he must assign $s_i$ streets for them to clean and a payment $p_i$ per street, taking into account the following guidelines:
* Each number of streets $s_i$ must be a positive integer and the sum $\sum_{i \in C} s_i$ must be exactly $S$, because every street must be cleaned and no street can be cleaned by more than one cleaner.
* Note that each hired cleaner $i$ will take $s_i \cdot H_i$ hours to clean their assigned streets. Since the cleaners can work in parallel, the cleaning job will take, in total, $\max_{i \in C} (s_i \cdot H_i)$ hours to be completed. This total time must be at most $K$ hours.
* The payment per street $p_i$ must be a rational number between $L_i$ and $U_i$ (that is, $L_i \le p_i \le U_i$).
* To ensure a fair hiring process, the payment per hour of work $p_i / H_i$ must be the same for all hired cleaners.

Notice that the above restrictions do not apply to the cleaners that are not hired.

Hired cleaner $i$ will receive $s_i \cdot p_i$ as payment, being the total payment $\sum_{i \in C} s_i \cdot p_i$. Help Oswaldo determine the minimum total payment for cleaning the streets honoring the given guidelines, or tell him that those requirements cannot be satisfied.

## Input

The first line contains three integers $N$, $S$ and $K$ ($1 \le N, S \le 10^5$ and $1 \le K \le 10^9$), indicating respectively the number of available cleaners, the number of streets to be cleaned, and in how many hours the job must be completed. Each cleaner is identified by a distinct integer from 1 to $N$.

The $i$-th of the next $N$ lines describes cleaner $i$ with three integers $H_i$ ($1 \le H_i \le 10^5$), $L_i$ and $U_i$ ($1 \le L_i \le U_i \le 100$), indicating that the cleaner can clean any street in $H_i$ hours, and accepts as payment any value in the range $[L_i, U_i]$.

## Output

If the given guidelines can be satisfied, output a single line with two positive integers $x$ and $y$, such that $x/y$ is an irreducible fraction indicating the minimum total payment for cleaning the streets according to those guidelines.
If the requirements cannot be fulfilled, output a line with the character `*` (asterisk) instead.

## Samples

| Sample input 1 | Sample output 1 |
|:---------------|:----------------|
| `2 15 10`<br>`1 4 10`<br>`2 2 8` | `80 1`          |

| Sample input 2 | Sample output 2 |
|:---------------|:----------------|
| `2 7 9`<br>`3 4 10`<br>`2 2 8` | `68 3`          |

| Sample input 3 | Sample output 3 |
|:---------------|:----------------|
| `2 15 10`<br>`1 4 10`<br>`5 2 8` | `*`             |