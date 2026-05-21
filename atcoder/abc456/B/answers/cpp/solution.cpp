#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<vector<int>> A(3, vector<int>(6));
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (!(cin >> A[i][j])) return 0;
        }
    }

    int match_count = 0;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            for (int k = 0; k < 6; ++k) {
                int x = A[0][i];
                int y = A[1][j];
                int z = A[2][k];
                
                if ((x == 4 && y == 5 && z == 6) ||
                    (x == 4 && y == 6 && z == 5) ||
                    (x == 5 && y == 4 && z == 6) ||
                    (x == 5 && y == 6 && z == 4) ||
                    (x == 6 && y == 4 && z == 5) ||
                    (x == 6 && y == 5 && z == 4)) {
                    match_count++;
                }
            }
        }
    }

    double probability = (double)match_count / 216.0;
    cout << fixed << setprecision(10) << probability << "\n";

    return 0;
}
