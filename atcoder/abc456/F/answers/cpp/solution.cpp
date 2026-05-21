#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

typedef long long ll;

void factorize(ll n, map<ll, ll>& factors) {
    for (ll i = 2; i * i <= n; ++i) {
        while (n % i == 0) {
            factors[i]++;
            n /= i;
        }
    }
    if (n > 1) factors[n]++;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    int n = s.length();
    
    map<ll, ll> factors;
    
    if (n <= 15) {
        ll val = stoll(s);
        factorize(val, factors);
    } else {
        // Take first 14 digits to keep sqrt(P) small enough for trial division
        ll P = stoll(s.substr(0, 14));
        int L = n - 14;
        factorize(P, factors);
        factors[2] += L;
        factors[5] += L;
    }
    
    vector<pair<ll, ll>> res;
    for (auto const& [p, e] : factors) {
        if (e > 0) res.push_back({p, e});
    }
    
    cout << res.size() << endl;
    for (auto const& p : res) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
