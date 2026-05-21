#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int X;
    if (cin >> X) {
        if (X >= 3 && X <= 18) {
            cout << "Yes\n";
        } else {
            cout << "No\n";
        }
    }
    return 0;
}
