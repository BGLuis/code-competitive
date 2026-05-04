#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    int count = 0;
    int curr_max = -1;
    for (int i = 0; i < N; ++i) {
        int d;
        cin >> d;
        if (d > curr_max) {
            count++;
            curr_max = d;
        }
    }
    cout << count << endl;

    return 0;
}
