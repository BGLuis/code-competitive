#include <iostream>
#include <string>

using namespace std;

int main() {
    // Optimize standard I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string S;
    if (!(cin >> S)) return 0;

    long long total = 0;
    long long mod = 998244353;
    long long L = 1;
    for (size_t i = 1; i < S.length(); ++i) {
        if (S[i] == S[i - 1]) {
            long long contribution = (L * (L + 1) / 2) % mod;
            total = (total + contribution) % mod;
            L = 1;
        } else {
            L++;
        }
    }
    long long contribution = (L * (L + 1) / 2) % mod;
    total = (total + contribution) % mod;

    cout << total << "\n";
    return 0;
}
