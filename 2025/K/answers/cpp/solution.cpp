#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    ll K;
    if (!(cin >> N >> K)) return 0;

    // If there's only one king, moving it just shifts the 1x1 bounding box.
    if (N == 1) {
        cout << 1 << endl;
        return 0;
    }

    vector<pair<ll, ll>> kings(N);
    ll r_min = 2e9, r_max = -2e9, c_min = 2e9, c_max = -2e9;
    for (int i = 0; i < N; ++i) {
        cin >> kings[i].first >> kings[i].second;
        if (kings[i].first < r_min) r_min = kings[i].first;
        if (kings[i].first > r_max) r_max = kings[i].first;
        if (kings[i].second < c_min) c_min = kings[i].second;
        if (kings[i].second > c_max) c_max = kings[i].second;
    }

    ll h0 = r_max - r_min + 1;
    ll w0 = c_max - c_min + 1;
    ll ans = h0 * w0;

    // Strategy 1: Corner moves. Use one king to expand both H and W directions.
    for (int i = 0; i < N; ++i) {
        ll r = kings[i].first;
        ll c = kings[i].second;
        ll au = r_max - r;
        ll ad = r - r_min;
        ll br = c_max - c;
        ll bl = c - c_min;

        ans = max(ans, (h0 + max(0LL, K - au)) * (w0 + max(0LL, K - br)));
        ans = max(ans, (h0 + max(0LL, K - au)) * (w0 + max(0LL, K - bl)));
        ans = max(ans, (h0 + max(0LL, K - ad)) * (w0 + max(0LL, K - br)));
        ans = max(ans, (h0 + max(0LL, K - ad)) * (w0 + max(0LL, K - bl)));
    }

    // Strategy 2: Orthogonal moves. Split K into height increase x and width increase (K-x).
    ll x_opt = (w0 + K - h0) / 2;
    for (ll x : {0LL, K, x_opt, x_opt + 1}) {
        if (x >= 0 && x <= K) {
            ans = max(ans, (h0 + x) * (w0 + K - x));
        }
    }

    cout << ans << endl;

    return 0;
}
