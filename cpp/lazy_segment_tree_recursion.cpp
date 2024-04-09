#include <vector>
#include <functional>
template <typename S, typename F> class lazy_segtree {
public:
  lazy_segtree(int _n) : n(_n), d(4 * _n), lz(4 * _n) {}
  lazy_segtree(std::vector<S> &v) : lazy_segtree(static_cast<int>(v.size())) {
    std::function<void(int, int, int)> build = [&](int p, int l, int r) {
      if (r - l == 1) {
        d[p] = v[l];
        return;
      }
      int m = (l + r) / 2;
      build(p << 1, l, m);
      build(p << 1 | 1, m, r);
      pull(p);
    };
    build(1, 0, n);
  }
  void modify(int p, int l, int r, int x, const S &v) {
    if (r - l == 1) {
      d[p] = v;
      return;
    }
    int m = (l + r) / 2;
    push(p);
    if (x < m) {
      modify(p << 1, l, m, x, v);
    } else {
      modify(p << 1 | 1, m, r, x, v);
    }
    pull(p);
  }
  void modify(int p, const S &v) { modify(1, 0, n, p, v); }
  S prod(int p, int l, int r, int x, int y) {
    if (l >= y || r <= x) {
      return S();
    }
    if (l >= x && r <= y) {
      return d[p];
    }
    int m = (l + r) / 2;
    push(p);
    return prod(p << 1, l, m, x, y) + prod(p << 1 | 1, m, r, x, y);
  }
  S prod(int l, int r) { return prod(1, 0, n, l, r); }
  S get(int p) { return prod(p, p + 1); }
  void apply(int p, int l, int r, int x, int y, const F &f) {
    if (l >= y || r <= x) {
      return;
    }
    if (l >= x && r <= y) {
      Apply(p, f);
      return;
    }
    int m = (l + r) / 2;
    push(p);
    apply(p << 1, l, m, x, y, f);
    apply(p << 1 | 1, m, r, x, y, f);
    pull(p);
  }
  void apply(int l, int r, const F &f) { apply(1, 0, n, l, r, f); }

private:
  int n;
  std::vector<S> d;
  std::vector<F> lz;
  void pull(int p) { d[p] = d[p << 1] + d[p << 1 | 1]; }
  void Apply(int p, const F &f) {
    d[p] = f(d[p]);
    lz[p] = lz[p] + f;
  }
  void push(int p) {
    Apply(p << 1, lz[p]);
    Apply(p << 1 | 1, lz[p]);
    lz[p] = F();
  }
};
using i64 = long long;
struct S {
  i64 x;
  S(i64 _x = 0) : x(_x) {}
  friend S operator+(S l, S r) { return S{l.x + r.x}; }
};

struct F {
  i64 add;
  F(i64 _add = 0) : add(_add) {}
  S operator()(S x) const { return S{x.x + add}; }
  friend F operator+(F l, F r) { return F{l.add + r.add}; }
};