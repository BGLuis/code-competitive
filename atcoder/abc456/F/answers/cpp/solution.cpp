#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Matrix {
    long long m0, m1, m2, m3;
};

inline Matrix multiply(const Matrix& A, const Matrix& B) {
    return {
        min(A.m0 + B.m0, A.m1 + B.m2),
        min(A.m0 + B.m1, A.m1 + B.m3),
        min(A.m2 + B.m0, A.m3 + B.m2),
        min(A.m2 + B.m1, A.m3 + B.m3)
    };
}

inline Matrix identity() {
    return {0, (long long)1e18, (long long)1e18, 0};
}

vector<long long> get_M_00(int N, int L, const vector<long long>& A) {
    vector<long long> res(N + 2, 0);
    if (L <= 0) return res;
    
    int num_blocks = (N + 2 * L) / L + 2;
    int max_size = num_blocks * L;
    vector<Matrix> T(max_size, identity());
    for (int i = 2; i <= N; ++i) {
        T[i] = {A[i], A[i], 0, (long long)1e18};
    }
    
    vector<Matrix> pref(max_size), suff(max_size);
    for (int b = 0; b < num_blocks; ++b) {
        int start = b * L;
        int end = (b + 1) * L - 1;
        
        pref[start] = T[start];
        for (int x = start + 1; x <= end; ++x) {
            pref[x] = multiply(T[x], pref[x-1]);
        }
        
        suff[end] = T[end];
        for (int x = end - 1; x >= start; --x) {
            suff[x] = multiply(suff[x+1], T[x]);
        }
    }
    
    for (int i = 1; i <= N - L; ++i) {
        int mid = ((i + L) / L) * L;
        Matrix prod;
        if (mid == i + 1) {
            prod = suff[i + 1];
        } else {
            prod = multiply(pref[i + L], suff[i + 1]);
        }
        res[i] = prod.m0;
    }
    return res;
}

void solve() {
    int N, K;
    if (!(cin >> N >> K)) return;
    vector<long long> A(N + 2);
    for (int i = 1; i <= N; ++i) {
        cin >> A[i];
    }
    A[0] = 1e18; // pad with infinity
    A[N + 1] = 1e18;
    
    if (K == 1) {
        long long ans = A[1];
        for (int i = 2; i <= N; ++i) {
            ans = min(ans, A[i]);
        }
        cout << ans << "\n";
        return;
    }
    
    vector<long long> M_K_minus_1 = get_M_00(N, K - 1, A);
    vector<long long> M_K_minus_2 = get_M_00(N, K - 2, A);
    
    long long global_ans = 1e18;
    for (int v = K; v <= N; ++v) {
        int i = v - K + 1;
        long long term1 = A[i] + M_K_minus_1[i];
        long long term2 = A[i-1] + M_K_minus_2[i+1] + A[i+1];
        global_ans = min({global_ans, term1, term2});
    }
    cout << global_ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    if (cin >> T) {
        while (T--) {
            solve();
        }
    }
    return 0;
}
