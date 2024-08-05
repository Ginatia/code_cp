template <typename S> struct segtree {
  int size;
  std::vector<S> d;
  void update(int k) { d[k] = d[2 * k + 1] + d[2 * k + 2]; }
  void init(int n) {
    size = 1;
    while (size < n)
      size *= 2;
    d.assign(2 * size, S());
  }
  void build(std::vector<S> &a, int x, int lx, int rx) {
    if (rx - lx == 1) {
      if (lx < (int)a.size()) {
        d[x] = a[lx];
      }
      return;
    }
    int mid = (lx + rx) / 2;
    build(a, 2 * x + 1, lx, mid);
    build(a, 2 * x + 2, mid, rx);
    update(x);
  }
  void build(std::vector<S> &a) { build(a, 0, 0, size); }

  segtree() {}
  explicit segtree(int n) { init(n); }
  explicit segtree(std::vector<S> &a) {
    init((int)a.size());
    build(a);
  }
  void set(int i, S v, int x, int lx, int rx) {
    if (rx - lx == 1) {
      d[x] = v;
      return;
    }
    int mid = (lx + rx) / 2;
    if (i < mid) {
      set(i, v, 2 * x + 1, lx, mid);
    } else {
      set(i, v, 2 * x + 2, mid, rx);
    }
    update(x);
  }
  void set(int i, S v) { set(i, v, 0, 0, size); }
  S prod(int l, int r, int x, int lx, int rx) {
    if (lx >= r || l >= rx)
      return S();
    if (lx >= l && rx <= r)
      return d[x];
    int mid = (lx + rx) / 2;
    S sl = prod(l, r, 2 * x + 1, lx, mid);
    S sr = prod(l, r, 2 * x + 2, mid, rx);
    return sl+sr;
  }
  S prod(int l, int r) { return prod(l, r, 0, 0, size); }
};

struct S {
  int x;
  S(int _x=(int)1e9) : x(_x) {}

  friend S operator+(S l, S r) { return S{std::min(l.x,r.x)}; }
};