# Problem J - Judgmental Crowd

After a glorious night of stand-up comedy, a comedian wants to measure their success. Not with surveys, not with likes, but with real crowd noises! Luckily, the mic captured all the reactions during the routine. The audio has been converted into a single string $S$ full of mashed-up sounds, such as applause, laughs, boos, etc.

The comedian uses a powerful tool for evaluating their show: the Instant Comedian Performance Calculator (ICPC). The ICPC is able to analyze $S$ and calculate the emotional score of the performance, which reflects how well the comedian did on stage.

The score is based on the occurrence of three specific substrings within the string $S$:
* each occurrence of "ha" increases the score by 1,
* each occurrence of "boooo" decreases the score by 1, and
* each occurrence of "bravo" increases the score by 3.

Any other substrings in $S$ (such as "ahhh", "woohoo" or random gibberish) are ignored and do not affect the score. Your mission is to simulate the ICPC by calculating the emotional score of the performance.

## Input

The input consists of a single line that contains a string $S$ ($1 \le |S| \le 1000$) made of lowercase letters, representing the sequence of crowd sounds picked up by the mic.

## Output

Output a single line with an integer indicating the emotional score of the performance.

## Samples

| Sample input 1 | Sample output 1 |
|:---------------|:----------------|
| `boooohaboooo` | `-1`            |

| Sample input 2 | Sample output 2 |
|:---------------|:----------------|
| `brhavo`       | `1`             |

| Sample input 3 | Sample output 3 |
|:---------------|:----------------|
| `booo`         | `0`             |

| Sample input 4 | Sample output 4 |
|:---------------|:----------------|
| `bravoooooobraboooooo` | `2`             |

| Sample input 5 | Sample output 5 |
|:---------------|:----------------|
| `buuuuuuthisshowisawfulweshallgonow` | `1`             |