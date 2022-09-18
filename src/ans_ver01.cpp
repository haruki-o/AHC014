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
  void out() { cout << x << " " << y << " "; }
  void debug_out() { cout << "(" << x << ", " << y << ") "; }
};

bool operator==(const Point &p1, const Point &p2) {
  return p1.x == p2.x && p1.y == p2.y;
}

struct Line {
  Point p1, p2;
  Point unit;
  Line() {}
  Line(Point &p1, Point &p2) : p1(p1), p2(p2) {
    ll unit_x = p2.x - p1.x;
    if (unit_x != 0)
      unit_x /= abs(unit_x);
    ll unit_y = p2.y - p1.y;
    if (unit_y != 0)
      unit_y /= abs(unit_y);
    unit = Point(unit_x, unit_y);
  }
  int line_type() {
    if (p1 == p2)
      return -1;
    if (p1.x == p2.x || p1.y == p2.y) {
      if (p1.x == p2.x)
        return (p2.y - p1.y) / abs(p2.y - p1.y) + 1;
      else
        return (p2.x - p1.x) / abs(p2.x - p1.x) + 2;
    }
    if (abs(p1.x - p2.x) == abs(p1.y - p2.y)) {
      int sum = 4;
      //第2，4象限
      if ((p1.x - p2.x) * (p1.y - p2.y) < 0)
        sum++;
      // 第3，4象限
      if (p2.y < p1.y)
        sum += 2;
      return sum;
    }
    return -1;
  }

  bool find_obstacle(ll N, vvi &ban) {
    ll sx = p1.x + unit.x;
    ll sy = p1.y + unit.y;
    while (sx != p2.x || sy != p2.y) {
      if (ban[sy][sx] == 1)
        return true;
      sx += unit.x;
      sy += unit.y;
    }
    return false;
  }

  void debug_out() {
    p1.debug_out();
    p2.debug_out();
  }
};

struct Rectangle {
  Point p1, p2, p3, p4;
  int type;
  Rectangle(Point p1, Point p2, Point p3, Point p4)
      : p1(p1), p2(p2), p3(p3), p4(p4) {
    type = Line(p1, p2).line_type();
  }

  void out() {
    p1.out();
    p2.out();
    p3.out();
    p4.out();
  }
};

struct Admin {
  ll N;
  //→ ↑ ⤴ ⤵
  vvvi used(4);
  Admin(ll N) {
    rep(i, 0, 4) {
      used[i].resize(N);
      rep(j, 0, N) used[i][j].assign(N, 0);
    }
  }
  Admin(vvi &u1, vvi &u2, vvi &u3, vvi &u4) {
    used[0] = u1;
    used[1] = u2;
    used[2] = u3;
    used[3] = u4;
  }
  bool is_set_line(Line L) {
    ll cux = L.p1.x;
    ll cuy = L.p1.y;
    //→↑
    if (L.line_type() < 4) {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        if (used[L.line_type() % 2][min(cuy, ney)][min(cux, nex)])
          return 0;
        // used[L.line_type() % 2][min(cuy, ney)][min(cux, nex)] = 1;
        cux = nex;
        cuy = ney;
      }
    } else {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        ll juy = L.line_type() % 2 == 0 ? min(cuy, ney) : max(cuy, ney);
        if (used[L.line_type() % 2 + 2][juy][min(cux, nex)])
          return 0;
        // used[L.line_type() % 2 + 2][juy][min(cux, nex)] = 1;
        cux = nex;
        cuy = ney;
      }
    }
  }
};

bool is_rectangle(Point &p1, Point &p2, Point &p3, Point &p4) {
  vector<Line> L(4);
  L[0] = Line(p1, p2);
  L[1] = Line(p2, p3);
  L[2] = Line(p3, p4);
  L[3] = Line(p4, p1);
  int p_type;
  int c_type = L[3].line_type();
  rep(i, 0, 4) {
    p_type = c_type;
    c_type = L[i].line_type();
    if (c_type == -1)
      return 0;
    if (c_type / 4 != p_type / 4 || c_type % 2 == p_type % 2)
      return 0;
  }
  return 1;
}

void solver(ll N, ll M, vector<Point> &ini, vector<Point> &find, vvi &ban) {
  ll sum = 0;
  rep(x, 0, N) {
    rep(y, 0, N) {
      Point p1 = Point(x, y);
      if (ban[y][x] == 1)
        continue;
      rep(p2, 0, M) {
        Line l1 = Line(p1, ini[p2]);
        if (l1.line_type() == -1)
          continue;
        if (l1.find_obstacle(N, ban) == true)
          continue;
        rep(p3, 0, M) {
          Line l2 = Line(ini[p2], ini[p3]);
          if (l2.line_type() == -1)
            continue;
          if (l2.find_obstacle(N, ban) == true)
            continue;
          rep(p4, 0, M) {
            Line l3 = Line(ini[p3], ini[p4]);
            if (l3.line_type() == -1)
              continue;
            if (l3.find_obstacle(N, ban) == true)
              continue;
            Line l4 = Line(ini[p4], p1);
            if (l4.line_type() == -1)
              continue;
            if (l4.find_obstacle(N, ban) == true)
              continue;
            if (is_rectangle(p1, ini[p2], ini[p3], ini[p4])) {
              sum++;
              if (l1.line_type() / 4 == 0) {
                p1.out();
                ini[p2].out();
                ini[p3].out();
                ini[p4].out();
                cout << endl;
              }
            }
            // cout << l1.line_type() << " ";
            // cout << l2.line_type() << " ";
            // cout << l3.line_type() << " ";
            // cout << l4.line_type() << " ";
            // cout << endl;
          }
        }
      }
    }
  }
  cout << sum << endl;
}

int main() {
  ll N, M;
  cin >> N >> M;
  vll x(M), y(M);
  rep(i, 0, M) cin >> x[i] >> y[i];
  vector<Point> initial_point(M);
  rep(i, 0, M) initial_point[i] = Point(x[i], y[i]);
  vvi ban(N, vi(N, 0));
  rep(i, 0, M) ban[y[i]][x[i]] = 1;

  vector<Rectangle> all_rectangle;
  vector<Point> all_point = initial_point;
  vector<Point> find_point;
  solver(N, M, initial_point, find_point, ban);
}
