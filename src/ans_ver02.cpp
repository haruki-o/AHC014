#include <atcoder/all>
#include <bits/stdc++.h>
using namespace std;
using namespace atcoder;
using namespace chrono;

typedef long long ll;
typedef pair<ll, ll> P;
typedef tuple<ll, ll, ll> T;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;
typedef vector<ll> vll;
typedef vector<vll> vvll;
#define rep(i, l, n) for (ll i = (ll)(l); i < (ll)(n); i++)
#define repd(i, n, l) for (ll i = (ll)(n); i > (ll)(l); i--)
#define TIME_LIMIT (4.7)
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
  ll length = 0;
  Rectangle() {}
  Rectangle(Point p1, Point p2, Point p3, Point p4)
      : p1(p1), p2(p2), p3(p3), p4(p4) {
    type = Line(p1, p2).line_type();
    if (type < 4) {
      length += (abs(p1.x - p2.x) + abs(p1.y - p2.y)) * 2;
      length += (abs(p2.x - p3.x) + abs(p2.y - p3.y)) * 2;
    } else {
      length += (abs(p1.x - p2.x) + abs(p1.y - p2.y));
      length += (abs(p2.x - p3.x) + abs(p2.y - p3.y));
    }
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
  vvvi used;
  Admin(ll N) {
    used.resize(4);
    rep(i, 0, 4) {
      used[i].resize(N);
      rep(j, 0, N) used[i][j].assign(N, 0);
    }
  }

  Admin(vvi &u1, vvi &u2, vvi &u3, vvi &u4) {
    used.resize(4);
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
    return true;
  }

  void set_line(Rectangle R) {
    vector<Line> _L(4);
    _L[0] = Line(R.p1, R.p2);
    _L[1] = Line(R.p2, R.p3);
    _L[2] = Line(R.p3, R.p4);
    _L[3] = Line(R.p4, R.p1);
    rep(i, 0, 4) {
      Line L = _L[i];
      ll cux = L.p1.x;
      ll cuy = L.p1.y;
      //→↑
      if (L.line_type() < 4) {
        while (!(Point(cux, cuy) == L.p2)) {
          ll nex = cux + L.unit.x;
          ll ney = cuy + L.unit.y;
          used[L.line_type() % 2][min(cuy, ney)][min(cux, nex)] = 1;
          cux = nex;
          cuy = ney;
        }
      } else {
        while (!(Point(cux, cuy) == L.p2)) {
          ll nex = cux + L.unit.x;
          ll ney = cuy + L.unit.y;
          ll juy = L.line_type() % 2 == 0 ? min(cuy, ney) : max(cuy, ney);
          used[L.line_type() % 2 + 2][juy][min(cux, nex)] = 1;
          cux = nex;
          cuy = ney;
        }
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

struct Solver {
  ll N, M;
  vector<Point> all_point;
  vvi ban;
  Admin admin;
  vector<Rectangle> ret;

  Solver(ll N, ll M, vector<Point> &all_point, vvi &ban, Admin &admin)
      : N(N), M(M), all_point(all_point), ban(ban), admin(admin) {}

  bool check_set_line(Line L) {
    if (L.line_type() == -1)
      return false;
    if (L.find_obstacle(N, ban) == true)
      return false;
    // if(L.p1 == Point(17,36), L.p2 == Point(20,33)){
    //   cout <<admin.is_set_line(L) << endl;
    // }
    if (admin.is_set_line(L) == false)
      return false;
    return true;
  }

  int calc_w(Point p) {
    ll _x = p.x - (N - 1) / 2;
    _x *= _x;
    ll _y = p.y - (N - 1) / 2;
    _y *= _y;
    return _x + _y + 1;
  }

  void search_all_point(vector<Rectangle> &_ret) {
    ll min_len = N * N;
    Rectangle best_r;
    rep(x, 0, N) {
      rep(y, 0, N) {
        Point p1(x, y);
        if (ban[y][x] == 1)
          continue;
        for (Point p2 : all_point) {
          Line l1(p1, p2);
          if (!check_set_line(l1))
            continue;
          for (Point p3 : all_point) {
            Line l2(p2, p3);
            if (!check_set_line(l2))
              continue;
            for (Point p4 : all_point) {
              Line l3(p3, p4);
              Line l4(p4, p1);
              if (!check_set_line(l3) || !check_set_line(l4))
                continue;
              if (is_rectangle(p1, p2, p3, p4)) {
                Rectangle r(p1, p2, p3, p4);
                if (r.length < min_len) {
                  best_r = r;
                  min_len = r.length;
                }
              }
            }
          }
        }
      }
    }
    if (min_len != N * N)
      _ret.push_back(best_r);
  }

  void set_Reactangle(Rectangle &R) {
    ban[R.p1.y][R.p1.x] = 1;
    admin.set_line(R);
    all_point.push_back(R.p1);
  }

  void solve() {
    srand((unsigned int)time(NULL));
    auto all_startClock = system_clock::now();
    while (1) {
      const double time =
          duration_cast<microseconds>(system_clock::now() - all_startClock)
              .count() *
          1e-6;
      if (time > TIME_LIMIT)
        break;
      vector<Rectangle> candi_r;
      search_all_point(candi_r);
      if (candi_r.empty()) {
        break;
      }
      set_Reactangle(candi_r[0]);
      ret.push_back(candi_r[0]);
    }
  }
};

void print_ans(vector<Rectangle> &res) {
  ll N = (ll)res.size();
  cout << N << endl;
  rep(i, 0, N) {
    res[i].out();
    cout << endl;
  }
}
int main() {
  ll N, M;
  cin >> N >> M;
  vll x(M), y(M);
  vector<Point> initial_point(M);
  vvi ban(N, vi(N, 0));
  rep(i, 0, M) cin >> x[i] >> y[i];
  rep(i, 0, M) initial_point[i] = Point(x[i], y[i]);
  rep(i, 0, M) ban[y[i]][x[i]] = 1;

  vector<Rectangle> all_rectangle;
  vector<Point> all_point = initial_point;
  Admin admin(N);
  Solver s(N, M, initial_point, ban, admin);
  s.solve();
  print_ans(s.ret);
}
