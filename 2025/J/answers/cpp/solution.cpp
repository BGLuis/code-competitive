#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    
    long long score = 0;
    int n = s.length();
    for (int i = 0; i < n; ++i) {
        // Checking for "ha"
        if (n - i >= 2 && s.compare(i, 2, "ha") == 0) score += 1;
        // Checking for "boooo"
        if (n - i >= 5 && s.compare(i, 5, "boooo") == 0) score -= 1;
        // Checking for "bravo"
        if (n - i >= 5 && s.compare(i, 5, "bravo") == 0) score += 3;
    }
    cout << score << endl;
    
    return 0;
}
