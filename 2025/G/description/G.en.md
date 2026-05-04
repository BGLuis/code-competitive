# Problem G - Gridoland Power Gauge

Gridoland is a city consisting of a grid of $N \times M$ blocks, designed for robust energy distribution. The city's power supply is a dam that can effectively generate an arbitrarily large current, and it is connected to the top-left block, at coordinates $(1, 1)$. The mayor is planning to host a big, energy-hungry celebration at the city hall, which is located at coordinates $(N, M)$, and she has hired you to help the planning committee.

Each block in Gridoland has its own energy transformer. Each transformer has an initial capacity to pass current, but this capacity changes linearly over time at a possibly different rate for each transformer. To help your job, technicians have determined each transformer's initial capacity $C_{i,j}$ and its rate of change $R_{i,j}$ (which can be positive, negative, or zero). Therefore, you know that the capacity of the transformer on block $(i, j)$ at minute $t$ will be $C_{i,j} + t \cdot R_{i,j}$.

Current flows within the city thanks to a number of power cables. All power cables in Gridoland are made of a superconductor material that can pass arbitrarily high current in either direction. The mayor herself handed you the city's connectivity map, containing all pairs of (vertically or horizontally) adjacent blocks that are connected by a power cable.

During the celebration there will be no energy consumption in any block except for the city hall, so all the current entering the energy grid will be used for the celebration. The celebration must happen at most $K$ minutes in the future, because the mayor's term will end at that time.

You must answer the following question to the planning committee: what will be the grid's peak capacity to supply the celebration at an integer number of minutes $t$ in the range $[0, K]$?

At each moment, the grid's capacity from the city hall's point of view is equal to the maximum amount of current that can flow from the power supply to the city hall. The current entering each transformer is limited by that transformer's capacity, and by the constraint that all of the current that enters a block has to leave (except for the city hall, which during the celebration will consume all energy it receives).

## Input

The first line contains four integers $N$, $M$, $P$ and $K$, ($2 \le N, M \le 300$, $P \ge 0$ and $0 \le K \le 10^{12}$). The values $N$ and $M$ indicate the city dimensions, $P$ represents the number of power cables in the city's connectivity map, and $K$ is the number of minutes left in the mayor's term.

Each of the next $N$ lines contains $M$ integers. In the $i$-th line, the $j$-th integer is $C_{i,j}$, the initial capacity of the transformer on block $(i, j)$ ($0 \le C_{i,j} \le 10^{12}$ for $i = 1, 2, \ldots, N$ and $j = 1, 2, \ldots, M$).

Each of the next $N$ lines contains $M$ integers. In the $i$-th line, the $j$-th integer is $R_{i,j}$, the rate of change of the transformer on block $(i, j)$ ($-10^6 \le R_{i,j} \le 10^6$ for $i = 1, 2, \ldots, N$ and $j = 1, 2, \ldots, M$).

Each of the next $P$ lines describes a bidirectional power cable with four integers $X_1$, $Y_1$, $X_2$ and $Y_2$ ($1 \le X_1, X_2 \le N$ and $1 \le Y_1, Y_2 \le M$), indicating that the cable connects block $(X_1, Y_1)$ with block $(X_2, Y_2)$.

It is guaranteed that all transformers have a non-negative capacity at every moment in the range $[0, K]$, that is, $C_{i,j} + t \cdot R_{i,j} \ge 0$ for every block $(i, j)$ and every $t \in [0, K]$.

It is guaranteed that each cable connects a different pair of blocks, and that any two blocks connected by a cable share a side.

## Output

Output a single line with an integer indicating the grid's peak capacity to supply the celebration at an integer number of minutes $t$ in the range $[0, K]$.

## Samples

| Sample input 1 | Sample output 1 |
|:---------------|:----------------|
| `2 2 3 10`<br>`5 4`<br>`5 6`<br>`0 0`<br>`0 0`<br>`2 1 1 1`<br>`1 1 1 2`<br>`1 2 2 2` | `4`             |

**Explanation of sample 1:**
In this case the city is a $2 \times 2$ grid, $K = 10$, and all blocks have a transformer with stable capacity because $R_{i,j} = 0$. The block $(1, 1)$ connected to the power supply has a transformer with capacity $C_{1,1} = 5$. It is connected to the transformer on block $(1, 2)$ which has capacity $C_{1,2} = 4$. This transformer is connected to the city hall's transformer on block $(2, 2)$, which has capacity $C_{2,2} = 6$. At any time between $t = 0$ and $t = K$ the energy grid can supply the city hall with a maximum current of 4.

| Sample input 2 | Sample output 2 |
|:---------------|:----------------|
| `2 3 6 12`<br>`25 1 18`<br>`10 2 50`<br>`0 2 -1`<br>`1 0 -2`<br>`1 1 1 2`<br>`1 2 1 3`<br>`1 3 2 3`<br>`1 1 2 1`<br>`2 1 2 2`<br>`2 2 2 3` | `14`            |

**Explanation of sample 2:**
Here the city is a $2 \times 3$ grid, with $K = 12$. There are two paths for energy to flow from block $(1, 1)$ to the city hall at $(2, 3)$. One of them uses the cables in the top row $(1, 1) \rightarrow (1, 2) \rightarrow (1, 3) \rightarrow (2, 3)$, and the other one uses the cables in the bottom row $(1, 1) \rightarrow (2, 1) \rightarrow (2, 2) \rightarrow (2, 3)$. The optimal time for the celebration is at $t = 6$, when a current of 12 can pass through the upper path, and an additional current of 2 can pass through the lower path.

| Sample input 3 | Sample output 3 |
|:---------------|:----------------|
| `2 2 1 1000000000000`<br>`5 4`<br>`5 6`<br>`0 0`<br>`0 0`<br>`2 1 1 1` | `0`             |