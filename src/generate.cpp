#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

typedef long long ll;
typedef vector<ll> vll;
#define rep(i, l, n) for (ll i = (ll)(l); i < (ll)(n); i++)

int main() {
  ll num;
  cin >> num;
  int sum = 0;
  for (ll N = 31; N <= 61; N += 4) {
    for (ll M = N; M <= N * N / 12; M += 10) {
      vector<pair<ll, ll>> point;
      rep(x, N / 4, 3 * N / 4) {
        rep(y, N / 4, 3 * N / 4) point.push_back({x, y});
      }
      rep(i, 0, num) {
        string S = ("0000" + to_string(sum));
        S = S.substr((int)S.size() - 4, 4);

        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());
        std::shuffle(point.begin(), point.end(), engine);

        fstream output_fstream;
        output_fstream.open("../in/" + S + ".txt", std::ios_base::out);

        output_fstream << N << " " << M << endl;
        rep(j, 0, M) output_fstream << point[j].first << " " << point[j].second
                                    << endl;
        sum++;
      }
    }
  }
}