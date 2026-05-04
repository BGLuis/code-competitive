#include <stdio.h>

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;

    int count = 0;
    int curr_max = -1;
    for (int i = 0; i < N; i++) {
        int d;
        scanf("%d", &d);
        if (d > curr_max) {
            count++;
            curr_max = d;
        }
    }
    printf("%d\n", count);

    return 0;
}
