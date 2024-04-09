#include <vector>
using i64 = long long;

template <typename S, typename F>
class lazy_segtree {
public:
  lazy_segtree() : lazy_segtree(0) {}
  explicit lazy_segtree(int n) : lazy_segtree(std::vector<S>(n, S())) {}
  explicit lazy_segtree(const std::vector<S> &v)
      : _n(static_cast<int>(v.size())) {
    size = static_cast<int>(bit_ceil(static_cast<unsigned int>(_n)));
    log = countr_zero(static_cast<unsigned int>(size));
    d = std::vector<S>(2 * size, S());
    lz = std::vector<F>(size, F());
    for (int i = 0; i < _n; i++) {
      d[i + size] = v[i];
    }
    for (int i = size - 1; i >= 1; i--) {
      update(i);
    }
  }
  void set(int p, S x) {
    p += size;
    for (int i = log; i >= 1; i--) {
      push(p >> i);
    }
    d[p] = x;
    for (int i = 1; i <= log; i++) {
      update(p >> i);
    }
  }
  S get(int p) {
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    return d[p];
  }
  S prod(int l, int r) {
    if (l == r)
      return S();

    l += size;
    r += size;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    S sml = S(), smr = S();
    while (l < r) {
      if (l & 1)
        sml = sml + d[l++];
      if (r & 1)
        smr = d[--r] + smr;
      l >>= 1;
      r >>= 1;
    }
    return sml + smr;
  }
  S all_prod() { return d[1]; }
  void apply(int p, F f) {
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    d[p] = f(d[p]);
    for (int i = 1; i <= log; i++)
      update(p >> i);
  }
  void apply(int l, int r, F f) {
    if (l == r)
      return;

    l += size;
    r += size;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    {
      int l2 = l, r2 = r;
      while (l < r) {
        if (l & 1)
          all_apply(l++, f);
        if (r & 1)
          all_apply(--r, f);
        l >>= 1;
        r >>= 1;
      }
      l = l2;
      r = r2;
    }

    for (int i = 1; i <= log; i++) {
      if (((l >> i) << i) != l)
        update(l >> i);
      if (((r >> i) << i) != r)
        update((r - 1) >> i);
    }
  }

  int _n, size, log;
  std::vector<S> d;
  std::vector<F> lz;
  void update(int k) { d[k] = d[2 * k] + d[2 * k + 1]; }
  void all_apply(int k, F f) {
    d[k] = f(d[k]);
    if (k < size) {
      lz[k] = lz[k] + f;
    }
  }
  void push(int k) {
    all_apply(k * 2, lz[k]);
    all_apply(k * 2 + 1, lz[k]);
    lz[k] = F();
  }

  static unsigned int bit_ceil(unsigned int n) {
    unsigned int x = 1;
    while (x < (unsigned int)(n))
      x *= 2;
    return x;
  }
  static int countr_zero(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x)))
      x++;
    return x;
  }
};
//a+b
struct S {
  i64 x, size;
  S() : x(0), size(1) {}
  S(i64 _x) : x(_x), size(1) {}
  S(i64 _x, i64 _size) : x(_x), size(_size) {}
  friend S operator+(S l, S r) { return S{l.x + r.x, l.size + r.size}; }
};
//add
struct F {
  i64 add;
  F(i64 _add = 0) : add(_add) {}
  S operator()(S x) { return S{x.x + add}; }
  friend F operator+(F l, F r) { return F{l.add + r.add}; }
};