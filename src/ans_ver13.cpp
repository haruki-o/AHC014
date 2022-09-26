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
#define TIME_LIMIT (4.9)
#define def (201010)
// #define MOD (1000000007)
#define MOD (998244353)
#define PI (3.14159265359)
#define INF (1 << 31)

struct Point {
  ll x, y;
  Point() {}
  Point(ll x, ll y) : x(x), y(y) {}
  bool is_internal(ll N) {
    if ((N / 4 <= x && x <= 3 * N / 4) && (N / 4 <= y && y <= 3 * N / 4))
      return true;
    return false;
  }
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
  vvvi used_e;
  vvi used_p;
  static constexpr int DX[8] = {1, 0, -1, 0, 1, -1, -1, 1};
  static constexpr int DY[8] = {0, 1, 0, -1, 1, 1, -1, -1};

  Admin(ll N, vector<Point> &all) : N(N) {
    used_e.resize(4);
    used_p.resize(N);
    rep(i, 0, 4) {
      used_e[i].resize(N);
      rep(j, 0, N) used_e[i][j].assign(N, 0);
    }
    rep(i, 0, N) used_p[i].assign(N, 0);
    for (Point p : all)
      used_p[p.y][p.x] = 1;
  }
  // Admin(Admin _ad){
  //   used_e = _ad.used_e;
  //   used_p = _ad.used_p;
  //   N = _ad.N;
  // }
  bool find_used_point(Line &L) {
    ll sx = L.p1.x + L.unit.x;
    ll sy = L.p1.y + L.unit.y;
    while (sx != L.p2.x || sy != L.p2.y) {
      if (used_p[sy][sx] == 1)
        return true;
      sx += L.unit.x;
      sy += L.unit.y;
    }
    return false;
  }

  bool find_used_edge(Line &L) {
    ll cux = L.p1.x;
    ll cuy = L.p1.y;
    //→↑
    if (L.line_type() < 4) {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        if (used_e[L.line_type() % 2][min(cuy, ney)][min(cux, nex)])
          return true;
        // used_e[L.line_type() % 2][min(cuy, ney)][min(cux, nex)] = 1;
        cux = nex;
        cuy = ney;
      }
    } else {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        ll juy = L.line_type() % 2 == 0 ? min(cuy, ney) : max(cuy, ney);
        if (used_e[L.line_type() % 2 + 2][juy][min(cux, nex)])
          return true;
        // used_e[L.line_type() % 2 + 2][juy][min(cux, nex)] = 1;
        cux = nex;
        cuy = ney;
      }
    }
    return false;
  }

  bool can_set_line(Line &L) {
    if (find_used_edge(L))
      return false;
    if (find_used_point(L))
      return false;
    return true;
  }

  bool can_set_rectangle(Rectangle &R) {
    Line l1(R.p1, R.p2);
    Line l2(R.p2, R.p3);
    Line l3(R.p3, R.p4);
    Line l4(R.p4, R.p1);
    if (!can_set_line(l1))
      return 0;
    if (!can_set_line(l2))
      return 0;
    if (!can_set_line(l3))
      return 0;
    if (!can_set_line(l4))
      return 0;
    return 1;
  }

  void set_line(Line &L) {
    ll cux = L.p1.x;
    ll cuy = L.p1.y;
    used_p[cuy][cux] = 1;
    //→↑
    if (L.line_type() < 4) {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        used_e[L.line_type() % 2][min(cuy, ney)][min(cux, nex)] = 1;
        cux = nex;
        cuy = ney;
      }
    } else {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        ll juy = L.line_type() % 2 == 0 ? min(cuy, ney) : max(cuy, ney);
        used_e[L.line_type() % 2 + 2][juy][min(cux, nex)] = 1;
        cux = nex;
        cuy = ney;
      }
    }
  }

  void set_rectangle(Rectangle &R) {
    Line l1(R.p1, R.p2);
    Line l2(R.p2, R.p3);
    Line l3(R.p3, R.p4);
    Line l4(R.p4, R.p1);
    set_line(l1);
    set_line(l2);
    set_line(l3);
    set_line(l4);
    used_p[R.p1.y][R.p1.x] = 1;
  }

  void delete_edge(Line &L) {
    ll cux = L.p1.x;
    ll cuy = L.p1.y;
    //→↑
    if (L.line_type() < 4) {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        used_e[L.line_type() % 2][min(cuy, ney)][min(cux, nex)] = 0;
        cux = nex;
        cuy = ney;
      }
    } else {
      while (!(Point(cux, cuy) == L.p2)) {
        ll nex = cux + L.unit.x;
        ll ney = cuy + L.unit.y;
        ll juy = L.line_type() % 2 == 0 ? min(cuy, ney) : max(cuy, ney);
        used_e[L.line_type() % 2 + 2][juy][min(cux, nex)] = 0;
        cux = nex;
        cuy = ney;
      }
    }
  }

  void delete_rectangle(Rectangle &R) {
    Line l1(R.p1, R.p2);
    Line l2(R.p2, R.p3);
    Line l3(R.p3, R.p4);
    Line l4(R.p4, R.p1);
    delete_edge(l1);
    delete_edge(l2);
    delete_edge(l3);
    delete_edge(l4);
    used_p[R.p1.y][R.p1.x] = 0;
  }

  Point find_point(Point &p, ll dir) {
    ll cux = p.x + DX[dir];
    ll cuy = p.y + DY[dir];
    Point _ret;
    while (1) {
      if (cux == -1 || cux == N || cuy == -1 || cuy == N) {
        return Point(-1, -1);
      }
      if (used_p[cuy][cux] == 1) {
        _ret = Point(cux, cuy);
        break;
      }
      cux += DX[dir];
      cuy += DY[dir];
    }
    Line L(p, _ret);
    if (find_used_edge(L))
      return Point(-1, -1);
    return _ret;
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

ll calc_score(ll N, ll M, vvi &used_p, vector<Rectangle> &R) {
  ll sum = 0;
  ll all = 0;
  ll c = (N - 1) / 2;
  for (Rectangle cu : R) {
    ll x = cu.p1.x;
    ll y = cu.p1.y;
    sum += (x - c) * (x - c) + (y - c) * (y - c) + 1;
  }

  rep(x, 0, N) {
    rep(y, 0, N) {
      if (used_p[y][x] == 1)
        sum += (x - c) * (x - c) + (y - c) * (y - c) + 1;
      all += (x - c) * (x - c) + (y - c) * (y - c) + 1;
    }
  }
  ll A = (ll)1000000 * N * N * sum;
  ll B = M * all;
  return A / B;
}

ll at_calc_score(ll N, ll M, vvi &used_p) {
  ll sum = 0;
  ll all = 0;
  ll c = (N - 1) / 2;
  rep(x, 0, N) {
    rep(y, 0, N) {
      if (used_p[y][x] == 1)
        sum += (x - c) * (x - c) + (y - c) * (y - c) + 1;
      all += (x - c) * (x - c) + (y - c) * (y - c) + 1;
    }
  }
  ll A = (ll)1000000 * N * N * sum;
  ll B = M * all;
  return A / B;
}

struct Solver {
  ll N, M;
  vector<Point> all_point;
  Admin admin;
  vector<Rectangle> ret;

  Solver(ll N, ll M, Admin &admin) : N(N), M(M), admin(admin) {}

  int calc_w(Point p) {
    ll _x = p.x - (N - 1) / 2;
    _x *= _x;
    ll _y = p.y - (N - 1) / 2;
    _y *= _y;
    return _x + _y + 1;
  }

  void search_all_point(vector<Rectangle> &_ret, ll turn) {
    ll max_w = -1;
    ll min_l = N * N;
    ll sum = 0;
    Rectangle best_r;
    rep(x, 0, N) {
      rep(y, 0, N) {
        Point p1(x, y);
        if (admin.used_p[y][x] == 1)
          continue;
        rep(dir, 0, 8) {
          Point p2 = admin.find_point(p1, dir);
          if (p2 == Point(-1, -1))
            continue;
          Point p3 = admin.find_point(p2, dir / 4 * 4 + (dir + 1) % 4);
          if (p3 == Point(-1, -1))
            continue;
          Point p4 = admin.find_point(p3, dir / 4 * 4 + (dir + 2) % 4);
          if (p4 == Point(-1, -1))
            continue;
          if (is_rectangle(p1, p2, p3, p4)) {
            Line l4(p4, p1);
            if (admin.can_set_line(l4)) {
              Rectangle _r(p1, p2, p3, p4);
              sum++;
              _ret.push_back(_r);
              //     if (max_w < calc_w(p1)) {
              //       best_r = _r;
              //       max_w = calc_w(p1);
              //     }
            }
          }
        }
      }
    }
    // if (max_w != -1 || min_l != N * N) {
    //   _ret.push_back(best_r);
    // }
    // cerr << turn << " " << sum << endl;
  }

  void modify(vector<Point> &_p, ll loop_num) {
    if (loop_num < 200) {
      std::random_device seed_gen;
      std::mt19937 engine(seed_gen());
      std::shuffle(_p.begin(), _p.end(), engine);
    } else {
      ll num = max((ll)5, 30 - loop_num / (ll)100);
      rep(k, 0, num) {
        ll i = rand() % (ll)_p.size();
        ll j = rand() % (ll)_p.size();
        swap(_p[i], _p[j]);
      }
    }
    // rep(num,0,3){
    //   ll i = rand() % (ll)_p.size();
    //   _p.erase(_p.begin() + i);
    // }
  }

  void solve() {
    srand((unsigned int)time(NULL));
    auto all_startClock = system_clock::now();

    vector<Point> all_point;
    vector<Rectangle> first_rectangle;
    search_all_point(first_rectangle, 0);
    vector<Rectangle> add_first_r;
    ll add_num = 0;
    while (1) {
      ll i = rand() % (ll)first_rectangle.size();
      if (admin.can_set_rectangle(first_rectangle[i])) {
        admin.set_rectangle(first_rectangle[i]);
        add_first_r.push_back(first_rectangle[i]);
        add_num++;
      }
      if (add_num == 3)
        break;
    }
    rep(i, 0, 3) {
      rep(x, N / 6, N - N / 6) {
        rep(y, N / 6, N - N / 6) all_point.push_back(Point(x, y));
      }
    }
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(all_point.begin(), all_point.end(), engine);
    ll loop_num = 0;
    ll best_score = 0;
    double start_temp = 20000, end_temp = 100; // 適当な値を入れる（後述）

    while (1) {
      const double time =
          duration_cast<microseconds>(system_clock::now() - all_startClock)
              .count() *
          1e-6;
      if (time > TIME_LIMIT)
        break;
      loop_num++;
      Admin _admin = admin;
      vector<Rectangle> _ret;
      vector<Point> new_all_point = all_point;
      modify(new_all_point, loop_num);
      ll score = 0;
      for (Point p1 : new_all_point) {
        rep(dir, 0, 8) {
          if (_admin.used_p[p1.y][p1.x] == 1)
            continue;
          Point p2 = _admin.find_point(p1, dir);
          if (p2 == Point(-1, -1))
            continue;
          Point p3 = _admin.find_point(p2, dir / 4 * 4 + (dir + 1) % 4);
          if (p3 == Point(-1, -1))
            continue;
          Point p4 = _admin.find_point(p3, dir / 4 * 4 + (dir + 2) % 4);
          if (p4 == Point(-1, -1))
            continue;
          if (is_rectangle(p1, p2, p3, p4)) {
            Line l4(p4, p1);
            if (_admin.can_set_line(l4)) {
              Rectangle _r(p1, p2, p3, p4);
              // if(_r.length >= 10)continue;
              _ret.push_back(_r);
              _admin.set_rectangle(_r);
            }
          }
        }
      }
      int new_score = at_calc_score(N, M, _admin.used_p);
      int pre_score = best_score;

      // 温度関数
      double temp = start_temp + (end_temp - start_temp) * time / TIME_LIMIT;
      // 遷移確率関数(最大化の場合)
      double prob = exp((new_score - pre_score) / temp);
      // cerr << temp << " " << new_score - pre_score << " " << prob << endl;
      if (prob > 0.9) { // 確率probで遷移する
        ret = _ret;
        best_score = new_score;
        all_point = new_all_point;
      }
      // cerr << new_score << " " << best_score << endl;
    }
    ret.insert(ret.begin(), add_first_r.begin(), add_first_r.end());
    cerr << duration_cast<microseconds>(system_clock::now() - all_startClock)
                    .count() *
                1e-6
         << " " << loop_num << endl;
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
  rep(i, 0, M) cin >> x[i] >> y[i];
  rep(i, 0, M) initial_point[i] = Point(x[i], y[i]);

  vector<Rectangle> all_rectangle;
  vector<Point> all_point = initial_point;
  Admin admin(N, all_point);
  Solver s(N, M, admin);
  s.solve();
  print_ans(s.ret);
  cerr << N << " " << M << " " << calc_score(N, M, s.admin.used_p, s.ret)
       << endl;
}
