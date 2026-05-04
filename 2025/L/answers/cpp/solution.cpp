#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef long double ld;

// Interval where the line is above the parabola
struct Interval {
    ld L, R;
    bool operator<(const Interval& other) const {
        if (abs(L - other.L) > 1e-11) return L < other.L;
        return R < other.R;
    }
};

struct FenwickTree {
    int n;
    vector<int> tree;
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int val) {
        for (; i <= n; i += i & -i) tree[i] += val;
    }
    int query(int i) {
        int res = 0;
        for (; i > 0; i -= i & -i) res += tree[i];
        return res;
    }
    int query(int l, int r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    ld A, B;
    if (!(cin >> N >> A >> B)) return 0;

    vector<Interval> intervals;
    for (int i = 0; i < N; ++i) {
        ld m, c;
        cin >> m >> c;
        // Intersection of y = mx + c and y = Ax^2 + B:
        // Ax^2 - mx + (B - c) = 0
        ld disc = m * m - 4.0 * A * (B - c);
        if (disc > 1e-11) {
            ld sd = sqrt(disc);
            ld l = (m - sd) / (2.0 * A);
            ld r = (m + sd) / (2.0 * A);
            intervals.push_back({l, r});
        }
    }

    if (intervals.empty()) {
        cout << 0 << endl;
        return 0;
    }

    // Sort intervals by left endpoint to use sweep-line
    sort(intervals.begin(), intervals.end());

    // Discretize R values for Fenwick tree
    vector<ld> all_R;
    for (const auto& in : intervals) all_R.push_back(in.R);
    sort(all_R.begin(), all_R.end());
    all_R.erase(unique(all_R.begin(), all_R.end(), [](ld a, ld b) {
        return abs(a - b) < 1e-11;
    }), all_R.end());

    int m_size = all_R.size();
    FenwickTree ft(m_size);

    ll ans = 0;
    for (const auto& in : intervals) {
        // Interlacing condition: L_j < L_i < R_j < R_i
        // Since we process in order of L_i, L_j < L_i is guaranteed.
        // We need to count previously inserted R_j such that L_i < R_j < R_i.
        
        int idx_low = lower_bound(all_R.begin(), all_R.end(), in.L + 1e-11) - all_R.begin() + 1;
        int idx_high = lower_bound(all_R.begin(), all_R.end(), in.R - 1e-11) - all_R.begin() + 1;
        
        ans += ft.query(idx_low, idx_high);

        int r_idx = lower_bound(all_R.begin(), all_R.end(), in.R - 1e-11) - all_R.begin() + 1;
        ft.add(r_idx, 1);
    }

    cout << ans << endl;

    return 0;
}
