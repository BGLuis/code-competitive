#include <stdio.h>
#include <string.h>

int main() {
    char s[1005];
    if (scanf("%s", s) != 1) return 0;
    
    long long score = 0;
    int n = strlen(s);
    for (int i = 0; i < n; i++) {
        if (i + 2 <= n && strncmp(s + i, "ha", 2) == 0) score += 1;
        if (i + 5 <= n && strncmp(s + i, "boooo", 5) == 0) score -= 1;
        if (i + 5 <= n && strncmp(s + i, "bravo", 5) == 0) score += 3;
    }
    printf("%lld\n", score);
    
    return 0;
}
