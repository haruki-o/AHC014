#include <atcoder/all>
#include <bits/stdc++.h>
using namespace std;
using namespace atcoder;

typedef long long ll;
typedef pair<ll, ll> P;
typedef tuple<ll, ll, ll> T;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ll> vll;
typedef vector<vll> vvll;
#define rep(i, l, n) for (ll i = (ll)(l); i < (ll)(n); i++)
#define repd(i, n, l) for (ll i = (ll)(n); i > (ll)(l); i--)
#define TIME_LIMIT (1.99)
#define def (201010)
// #define MOD (1000000007)
#define MOD (998244353)
#define PI (3.14159265359)

struct Point {
  ll x, y;
  Point() {}
  Point(ll x, ll y) : x(x), y(y) {}
  void out() { cout << "(" << x << ", " << y << ") "; }
};

bool operator==(const Point &p1, const Point &p2) {
  return p1.x == p2.x && p1.y == p2.y;
}

int line_type(Point p1, Point p2) {
  if (p1 == p2)
    return -1;
  if (p1.x == p2.x || p1.y == p2.y) {
    if (p1.x == p2.x)
      return (p2.y - p1.y) / abs(p2.y - p1.y) * 2;
    else
      return (p2.x - p1.x) / abs(p2.x - p1.x) * 2 + 1;
  }
  if (abs(p1.x - p2.x) == abs(p1.y - p2.y)) {
    int sum = 4;
    //第2，4象限
    if ((p1.x - p2.x) * (p1.y - p2.y) < 0)
      sum++;
    // 第3，4象限
    if ((p2.y < p1.y))
      sum += 2;
    return sum;
  }
  return -1;
}

struct Rectangle {
  Point p1, p2, p3, p4;
  int type;
  Rectangle(Point p1, Point p2, Point p3, Point p4)
      : p1(p1), p2(p2), p3(p3), p4(p4) {
    type = line_type(p1, p2);
  }

  void out() {
    p1.out();
    p2.out();
    p3.out();
    p4.out();
  }
};

bool is_rectangle(Point p1, Point p2, Point p3, Point p4) {
  if (p1 == Point(9, 15) && p2 == Point(12, 18) && p3 == Point(15, 15) &&
      p4 == Point(12, 12)) {
    p1.out();
    p2.out();
    p3.out();
    p4.out();
    cout << endl;
  }
  int type = line_type(p1, p2);
  if (type == -1)
    return 0;
  if (p1 == Point(9, 15) && p2 == Point(12, 18) && p3 == Point(15, 15) &&
      p4 == Point(12, 12)) {
    cout << "b" << endl;
  }
  
  if (line_type(p2, p3) == -1 || type / 4 != line_type(p2, p3) / 4 ||
      type % 2 == line_type(p2, p3) % 2)
    return 0;
  if (p1 == Point(9, 15) && p2 == Point(12, 18) && p3 == Point(15, 15) &&
      p4 == Point(12, 12)) {
    cout << "a" << endl;
  }
  if (line_type(p3, p4) == -1 || type / 4 != line_type(p3, p4) / 4 ||
      type % 2 == line_type(p3, p4) % 2)
    return 0;
  if (line_type(p4, p1) == -1 || type / 4 != line_type(p4, p1) / 4 ||
      type % 2 == line_type(p4, p1) % 2)
    return 0;
  return 1;
}

void solver(ll N, ll M, vector<Point> &initial, vector<Point> &find) {
  rep(x, 0, N) {
    rep(y, 0, N) {
      Point cu_point = Point(x, y);
      rep(p2, 0, M) {
        rep(p3, 0, M) {
          rep(p4, 0, M) {
            if (p2 == p3 || p3 == p4 || p4 == p2)
              continue;
            if (initial[p2] == cu_point || initial[p3] == cu_point ||
                initial[p4] == cu_point)
              continue;
            if (cu_point == Point(9, 15) && p2 == 18 && p3 == 34 && p4 == 49) {
              cout << p2 << " " << p3 << " " << p4 << endl;
              cout << line_type(cu_point, initial[p2]) << " "
                   << line_type(initial[p2], initial[p3]) << " "
                   << line_type(initial[p3], initial[p4]) << " "
                   << line_type(initial[p4], cu_point) << " " << endl;
              cout << is_rectangle(cu_point, initial[p2], initial[p3],
                                   initial[p4])
                   << endl;
            }
            if (is_rectangle(cu_point, initial[p2], initial[p3], initial[p4])) {
              cout << p2 << " " << p3 << " " << p4 << endl;
              initial[p2].out();
              initial[p3].out();
              initial[p4].out();
              cout << endl;
              cout << line_type(cu_point, initial[p2]) << " "
                   << line_type(initial[p2], initial[p3]) << " "
                   << line_type(initial[p3], initial[p4]) << " "
                   << line_type(initial[p4], cu_point) << " " << endl;
              Rectangle(cu_point, initial[p2], initial[p3], initial[p4]).out();
              cout << endl;
            }
          }
        }
      }
    }
  }
}

int main() {
  ll N, M;
  cin >> N >> M;
  vll x(M), y(M);
  rep(i, 0, M) cin >> x[i] >> y[i];
  vector<Point> initial_point(M);
  rep(i, 0, M) initial_point[i] = Point(x[i], y[i]);

  vector<Rectangle> all_rectangle;
  vector<Point> all_point = initial_point;
  vector<Point> find_point;
  solver(N, M, initial_point, find_point);
}
