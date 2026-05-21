# Linear Scan String Matching (KMP Algorithm)

## Conceptual Explanation

String matching (or string searching) is the problem of finding all occurrences of a pattern string $P$ of length $M$ inside a text string $T$ of length $N$.

### The Naive Search ($\mathcal{O}(N \cdot M)$)
The naive approach scans the text linearly, trying to align the pattern starting from each character index of the text. At any mismatch, it resets the pattern alignment pointer completely and starts over at the next text index. This leads to redundant character comparisons.

### Knuth-Morris-Pratt (KMP) Algorithm ($\mathcal{O}(N + M)$)
The **KMP algorithm** optimizes this process by ensuring that we never backtrack the text pointer. When a character mismatch occurs, we use precomputed information about the pattern itself to know how far we can safely shift the pattern without missing potential matches.

#### 1. The Prefix Function ($\pi$-table / LPS array)
To do this, we precompute a **longest proper prefix which is also a suffix (LPS)** array of size $M$.
- `LPS[i]` stores the length of the longest proper prefix of the substring $P[0...i]$ that is also a suffix of $P[0...i]$.

*Example*: For $P = \text{"abacaba"}$:
- The proper prefixes of "abacaba" are: "a", "ab", "aba", "abac", "abaca", "abacab".
- The suffixes are: "a", "ba", "aba", "caba", "acaba", "bacaba".
- The longest proper prefix that is also a suffix is "aba" (length 3).

#### 2. Pattern Matching with the LPS Array
During the linear scan of text $T$:
- We maintain a pattern index pointer `j`.
- If $T[i] == P[j]$, we increment both pointers.
- If $j == M$, a match is found. We then shift the pattern by updating $j = LPS[j-1]$.
- If a mismatch occurs ($T[i] \neq P[j]$):
  - If $j > 0$, instead of resetting $j$ to 0 and shifting $i$ back, we shift $j$ to $LPS[j-1]$ and retry the comparison at $T[i]$.
  - If $j == 0$, we simply increment $i$ to check the next text character.

---

## Complexity Analysis

- **Time Complexity**:
  - **LPS Construction**: $\mathcal{O}(M)$ time, as the pointer `j` only increases or decreases along the pattern in an amortized linear fashion.
  - **Search Scan**: $\mathcal{O}(N)$ time. The text pointer `i` only moves forward, and the pattern pointer `j` undergoes at most $N$ decreases.
  - **Total Time**: $\mathcal{O}(N + M)$ time.
- **Space Complexity**:
  - $\mathcal{O}(M)$ auxiliary space to store the LPS array.

---

## C++ Implementation

Below is a complete and robust C++ implementation of the KMP algorithm returning 0-based indices of all starting matches.

```cpp
#include <iostream>
#include <vector>
#include <string>

class KMPMatcher {
private:
    std::string pattern;
    std::vector<int> lps;

    // Precomputes the Longest Prefix Suffix (LPS) table
    void compute_lps() {
        int m = pattern.length();
        lps.assign(m, 0);
        int len = 0; // Length of the previous longest prefix suffix
        int i = 1;

        while (i < m) {
            if (pattern[i] == pattern[len]) {
                len++;
                lps[i] = len;
                i++;
            } else {
                if (len != 0) {
                    len = lps[len - 1]; // Fallback to previous best length
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
    }

public:
    explicit KMPMatcher(const std::string& pat) : pattern(pat) {
        compute_lps();
    }

    // Searches for occurrences of the pattern in the given text
    std::vector<int> search(const std::string& text) const {
        std::vector<int> match_indices;
        int n = text.length();
        int m = pattern.length();

        if (m == 0) return {};

        int i = 0; // Text index pointer
        int j = 0; // Pattern index pointer

        while (i < n) {
            if (text[i] == pattern[j]) {
                i++;
                j++;
            }

            if (j == m) {
                // Match found! Record starting index
                match_indices.push_back(i - j);
                j = lps[j - 1];
            } else if (i < n && text[i] != pattern[j]) {
                // Mismatch after j matches
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }

        return match_indices;
    }
};
```

---

## Python Implementation

Here is the Python 3 KMP implementation with complete type hints.

```python
from typing import List

class KMPMatcher:
    def __init__(self, pattern: str):
        self.pattern = pattern
        self.lps: List[int] = []
        self._compute_lps()

    def _compute_lps(self) -> None:
        m = len(self.pattern)
        self.lps = [0] * m
        length = 0  # Length of the previous longest prefix suffix
        i = 1

        while i < m:
            if self.pattern[i] == self.pattern[length]:
                length += 1
                self.lps[i] = length
                i += 1
            else:
                if length != 0:
                    length = self.lps[length - 1]
                else:
                    self.lps[i] = 0
                    i += 1

    def search(self, text: str) -> List[int]:
        """
        Searches for pattern occurrences in text.
        Returns a list of starting indices.
        """
        match_indices = []
        n = len(text)
        m = len(self.pattern)

        if m == 0:
            return []

        i = 0  # Text index pointer
        j = 0  # Pattern index pointer

        while i < n:
            if text[i] == self.pattern[j]:
                i += 1
                j += 1

            if j == m:
                match_indices.append(i - j)
                j = self.lps[j - 1]
            elif i < n and text[i] != self.pattern[j]:
                if j != 0:
                    j = self.lps[j - 1]
                else:
                    i += 1

        return match_indices
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    std::string text = "ABABDABACDABABCABAB";
    std::string pattern = "ABABCABAB";

    KMPMatcher matcher(pattern);
    std::vector<int> occurrences = matcher.search(text);

    std::cout << "Text:    " << text << "\n";
    std::cout << "Pattern: " << pattern << "\n\n";
    
    std::cout << "Found pattern at starting index/indices: ";
    for (int idx : occurrences) {
        std::cout << idx << " ";
    }
    std::cout << "(Expected: 10)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    text = "ABABDABACDABABCABAB"
    pattern = "ABABCABAB"

    matcher = KMPMatcher(pattern)
    occurrences = matcher.search(text)

    print(f"Text:    {text}")
    print(f"Pattern: {pattern}\n")
    print(f"Found pattern at starting index/indices: {occurrences} (Expected: [10])")
```
