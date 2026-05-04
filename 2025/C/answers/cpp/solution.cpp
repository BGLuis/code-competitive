#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef __int128_t int128;

long long gcd(long long a, long long b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}

bool is_better(long long n1, long long d1, long long n2, long long d2) {
    if (n2 == -1) return true;
    return (int128)n1 * d2 < (int128)n2 * d1;
}

struct Node {
    long long cnt_M;
    long long sum_MH;
};

int N_tree;
vector<Node> tree;
vector<int> distinct_H;

void update_tree(int idx, long long val_M, long long val_MH) {
    idx += N_tree;
    tree[idx].cnt_M += val_M;
    tree[idx].sum_MH += val_MH;
    while (idx > 1) {
        idx /= 2;
        tree[idx].cnt_M = tree[2 * idx].cnt_M + tree[2 * idx + 1].cnt_M;
        tree[idx].sum_MH = tree[2 * idx].sum_MH + tree[2 * idx + 1].sum_MH;
    }
}

long long query_tree(long long S) {
    int idx = 1;
    long long res = 0;
    int l = 0, r = N_tree - 1;
    while (l < r) {
        int mid = (l + r) / 2;
        if (tree[2 * idx].cnt_M >= S) {
            idx = 2 * idx;
            r = mid;
        } else {
            res += tree[2 * idx].sum_MH;
            S -= tree[2 * idx].cnt_M;
            idx = 2 * idx + 1;
            l = mid + 1;
        }
    }
    res += S * distinct_H[l];
    return res;
}

struct Cleaner {
    int H, L, U;
    long long M;
};

struct Rational {
    int num, den;
    bool operator<(const Rational& other) const {
        return (long long)num * other.den < (long long)other.num * den;
    }
    bool operator==(const Rational& other) const {
        return (long long)num * other.den == (long long)other.num * den;
    }
};

struct Event {
    Rational coord;
    int type; // 0 for start, 1 for end
    int id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long S, K;
    if (!(cin >> N >> S >> K)) return 0;

    vector<Cleaner> cleaners;
    vector<Event> events;
    for (int i = 0; i < N; ++i) {
        int h, l, u;
        cin >> h >> l >> u;
        long long m = K / h;
        if (m >= 1) {
            int id = cleaners.size();
            cleaners.push_back({h, l, u, m});
            events.push_back({{l, h}, 0, id});
            events.push_back({{u, h}, 1, id});
            distinct_H.push_back(h);
        }
    }

    if (cleaners.empty()) {
        cout << "*" << endl;
        return 0;
    }

    sort(distinct_H.begin(), distinct_H.end());
    distinct_H.erase(unique(distinct_H.begin(), distinct_H.end()), distinct_H.end());

    N_tree = 1;
    while (N_tree < (int)distinct_H.size()) N_tree *= 2;
    tree.assign(2 * N_tree, {0, 0});

    sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        if (!(a.coord == b.coord)) return a.coord < b.coord;
        return a.type < b.type;
    });

    long long best_num = -1, best_den = 1;

    int event_ptr = 0;
    while (event_ptr < (int)events.size()) {
        Rational current_R = events[event_ptr].coord;
        
        int temp_ptr = event_ptr;
        while (temp_ptr < (int)events.size() && events[temp_ptr].coord == current_R) {
            if (events[temp_ptr].type == 0) {
                int id = events[temp_ptr].id;
                int h_idx = lower_bound(distinct_H.begin(), distinct_H.end(), cleaners[id].H) - distinct_H.begin();
                update_tree(h_idx, cleaners[id].M, (long long)cleaners[id].M * cleaners[id].H);
            }
            temp_ptr++;
        }
        
        if (tree[1].cnt_M >= S) {
            long long sumH = query_tree(S);
            long long cur_num = (long long)current_R.num * sumH;
            long long cur_den = current_R.den;
            if (is_better(cur_num, cur_den, best_num, best_den)) {
                best_num = cur_num;
                best_den = cur_den;
            }
        }
        
        while (event_ptr < (int)events.size() && events[event_ptr].coord == current_R) {
            if (events[event_ptr].type == 1) {
                int id = events[event_ptr].id;
                int h_idx = lower_bound(distinct_H.begin(), distinct_H.end(), cleaners[id].H) - distinct_H.begin();
                update_tree(h_idx, -cleaners[id].M, -(long long)cleaners[id].M * cleaners[id].H);
            }
            event_ptr++;
        }
    }

    if (best_num == -1) {
        cout << "*" << endl;
    } else {
        long long common = gcd(best_num, best_den);
        cout << best_num / common << " " << best_den / common << endl;
    }

    return 0;
}
