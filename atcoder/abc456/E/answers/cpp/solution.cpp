#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e16;

struct Node {
    long long min_val;
    long long lazy;
};

int n_tree;
vector<Node> tree;

void push(int node) {
    if (tree[node].lazy != 0) {
        tree[2 * node].min_val += tree[node].lazy;
        tree[2 * node].lazy += tree[node].lazy;
        tree[2 * node + 1].min_val += tree[node].lazy;
        tree[2 * node + 1].lazy += tree[node].lazy;
        tree[node].lazy = 0;
    }
}

void update_range(int node, int l, int r, int ql, int qr, int val) {
    if (ql > r || qr < l) return;
    if (ql <= l && r <= qr) {
        tree[node].min_val += val;
        tree[node].lazy += val;
        return;
    }
    push(node);
    int mid = (l + r) / 2;
    update_range(2 * node, l, mid, ql, qr, val);
    update_range(2 * node + 1, mid + 1, r, ql, qr, val);
    tree[node].min_val = min(tree[2 * node].min_val, tree[2 * node + 1].min_val);
}

void update_point(int node, int l, int r, int idx, long long val) {
    if (l == r) {
        tree[node].min_val = val + tree[node].lazy;
        return;
    }
    push(node);
    int mid = (l + r) / 2;
    if (idx <= mid) update_point(2 * node, l, mid, idx, val);
    else update_point(2 * node + 1, mid + 1, r, idx, val);
    tree[node].min_val = min(tree[2 * node].min_val, tree[2 * node + 1].min_val);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int Q;
    if (!(cin >> Q)) return 0;
    vector<int> X(Q);
    vector<int> distinct_vals;
    for (int i = 0; i < Q; ++i) {
        cin >> X[i];
        distinct_vals.push_back(abs(X[i]));
    }
    sort(distinct_vals.begin(), distinct_vals.end());
    distinct_vals.erase(unique(distinct_vals.begin(), distinct_vals.end()), distinct_vals.end());

    int m = distinct_vals.size();
    if (m == 0) {
        for (int i = 0; i < Q; i++) cout << 0 << (i == Q - 1 ? "" : " ");
        cout << endl;
        return 0;
    }

    n_tree = 1;
    while (n_tree < m) n_tree *= 2;
    tree.assign(2 * n_tree, {INF, 0});

    vector<int> counts(m, 0);
    int current_n = 0;
    vector<int> results;

    for (int i = 0; i < Q; ++i) {
        int val = abs(X[i]);
        int idx = lower_bound(distinct_vals.begin(), distinct_vals.end(), val) - distinct_vals.begin();
        if (X[i] > 0) {
            if (counts[idx] == 0) {
                update_point(1, 0, n_tree - 1, idx, (long long)val);
            }
            counts[idx]++;
            current_n++;
            update_range(1, 0, n_tree - 1, idx, m - 1, -1);
        } else {
            update_range(1, 0, n_tree - 1, idx, m - 1, 1);
            counts[idx]--;
            current_n--;
            if (counts[idx] == 0) {
                update_point(1, 0, n_tree - 1, idx, INF);
            }
        }
        
        long long tree_min = tree[1].min_val;
        long long ans = current_n;
        if (tree_min < 0) {
            ans += tree_min;
        }
        results.push_back((int)ans);
    }

    for (int i = 0; i < Q; ++i) {
        cout << results[i] << (i == Q - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}
