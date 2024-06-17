#include <bits/stdc++.h>
#include <vector>
using i64 = long long;

template <typename T, T MOD = 998244353> struct Mint {
  inline static constexpr T mod = MOD;
  T v;
  Mint() : v(0) {}
  Mint(long long t) {
    v = t % MOD;
    if (v < 0)
      v += MOD;
  }
  constexpr i64 get() const { return v; }
  constexpr int get_mod() const { return MOD; }

  Mint pow(long long k) {
    Mint res(1), tmp(v);
    while (k) {
      if (k & 1)
        res *= tmp;
      tmp *= tmp;
      k >>= 1;
    }
    return res;
  }

  static Mint add_identity() { return Mint(0); }
  static Mint mul_identity() { return Mint(1); }

  Mint inv() { return pow(MOD - 2); }

  Mint &operator+=(Mint a) {
    v += a.v;
    if (v >= MOD)
      v -= MOD;
    return *this;
  }
  Mint &operator-=(Mint a) {
    v -= a.v;
    if (v < 0)
      v += MOD;
    return *this;
  }
  Mint &operator*=(Mint a) {
    v = 1LL * v * a.v % MOD;
    return *this;
  }
  Mint &operator/=(Mint a) { return (*this) *= a.inv(); }

  Mint operator+(Mint a) const { return Mint(v) += a; }
  Mint operator-(Mint a) const { return Mint(v) -= a; }
  Mint operator*(Mint a) const { return Mint(v) *= a; }
  Mint operator/(Mint a) const { return Mint(v) /= a; }

  Mint operator+() const { return *this; }
  Mint operator-() const { return v ? Mint(MOD - v) : Mint(v); }

  bool operator==(const Mint a) const { return v == a.v; }
  bool operator!=(const Mint a) const { return v != a.v; }
};
template <typename T, T MOD>
std::ostream &operator<<(std::ostream &os, Mint<T, MOD> m) {
  os << m.v;
  return os;
}

namespace NTT {
using i64 = long long;

i64 mod_pow(i64 a, i64 n, i64 mod) {
  i64 ans = 1;
  while (n > 0) {
    if (n & 1)
      ans = ans * a % mod;
    a = a * a % mod;
    n >>= 1;
  }
  return ans;
}
i64 mod_inv(i64 a, i64 mod) {
  i64 b = mod, u = 1, v = 0;
  while (b) {
    i64 t = a / b;
    a -= t * b;
    std::swap(a, b);
    u -= t * v;
    std::swap(u, v);
  }
  u %= mod;
  if (u < 0)
    u += mod;
  return u;
}

int calc_primitive_root(int mod) {
  if (mod == 2)
    return 1;
  if (mod == 167772161)
    return 3;
  if (mod == 469762049)
    return 3;
  if (mod == 754974721)
    return 11;
  if (mod == 998244353)
    return 3;

  int divs[20] = {};
  divs[0] = 2;
  int cnt = 1;
  i64 x = (mod - 1) / 2;
  while (x % 2 == 0)
    x /= 2;
  for (i64 i = 3; i * i <= x; i += 2) {
    if (x % i == 0) {
      divs[cnt++] = i;
      while (x % i == 0)
        x /= i;
    }
  }
  if (x > 1)
    divs[cnt++] = x;
  for (int g = 2;; g++) {
    bool ok = true;
    for (int i = 0; i < cnt; i++) {
      if (mod_pow(g, (mod - 1) / divs[i], mod) == 1) {
        ok = false;
        break;
      }
    }
    if (ok)
      return g;
  }
}
int get_fft_size(int N, int M) {
  int size_a = 1, size_b = 1;
  while (size_a < N)
    size_a <<= 1;
  while (size_b < M)
    size_b <<= 1;
  return std::max(size_a, size_b) << 1;
}

template <typename Z> void trans(std::vector<Z> &v, bool inv = false) {
  if (v.empty())
    return;
  int N = (int)v.size();
  int MOD = v[0].get_mod();
  int PR = calc_primitive_root(MOD);

  static bool first = true;
  static std::vector<i64> vbw(30), vibw(30);
  if (first) {
    first = false;
    for (int k = 0; k < 30; k++) {
      vbw[k] = mod_pow(PR, (MOD - 1) >> (k + 1), MOD);
      vibw[k] = mod_inv(vbw[k], MOD);
    }
  }

  for (int i = 0, j = 1; j < N - 1; j++) {
    for (int k = N >> 1; k > (i ^= k); k >>= 1)
      ;
    if (i > j)
      std::swap(v[i], v[j]);
  }
  for (int k = 0, t = 2; t <= N; ++k, t <<= 1) {
    i64 bw = vbw[k];
    if (inv)
      bw = vibw[k];
    for (int i = 0; i < N; i += t) {
      Z w = 1;
      for (int j = 0; j < t / 2; j++) {
        int j1 = i + j, j2 = i + j + t / 2;
        Z c1 = v[j1], c2 = v[j2] * w;
        v[j1] = c1 + c2;
        v[j2] = c1 - c2;
        w *= bw;
      }
    }
  }
  if (inv) {
    i64 invN = mod_inv(N, MOD);
    for (int i = 0; i < N; i++)
      v[i] = v[i] * invN;
  }
}

// garner
static constexpr int MODs[] = {754974721, 167772161, 469762049};
using Z0 = Mint<i64, MODs[0]>;
using Z1 = Mint<i64, MODs[1]>;
using Z2 = Mint<i64, MODs[2]>;
static const Z1 imod0 = 95869806;
static const Z2 imod1 = 104391568;
static const Z2 imod01 = 187290749;

template <typename T>
std::vector<T> native_mul(const std::vector<T> &A, const std::vector<T> &B) {
  if (A.empty() || B.empty())
    return {};
  int N = (int)A.size(), M = (int)B.size();
  std::vector<T> ans(N + M - 1);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++) {
      ans[i + j] += A[i] * B[j];
    }
  return ans;
}

template <typename Z>
std::vector<Z> mul(const std::vector<Z> &A, const std::vector<Z> &B) {
  if (A.empty() || B.empty())
    return {};
  int N = (int)A.size(), M = (int)B.size();

  if (std::min(N, M) < 30)
    return native_mul(A, B);

  int MOD = A[0].get_mod();
  int size_fft = get_fft_size(N, M);

  if (MOD == 998244353) {
    std::vector<Z> a(size_fft), b(size_fft), c(size_fft);
    for (int i = 0; i < N; i++)
      a[i] = A[i];
    for (int i = 0; i < M; i++)
      b[i] = B[i];
    trans(a);
    trans(b);
    std::vector<Z> res(size_fft);
    for (int i = 0; i < size_fft; i++)
      res[i] = a[i] * b[i];
    trans(res, true);
    res.resize(N + M - 1);
    return res;
  }

  std::vector<Z0> a0(size_fft, 0), b0(size_fft, 0), c0(size_fft, 0);
  std::vector<Z1> a1(size_fft, 0), b1(size_fft, 0), c1(size_fft, 0);
  std::vector<Z2> a2(size_fft, 0), b2(size_fft, 0), c2(size_fft, 0);
  for (int i = 0; i < N; i++) {
    a0[i] = A[i].v, a1[i] = A[i].v, a2[i] = A[i].v;
  }
  for (int i = 0; i < M; i++) {
    b0[i] = B[i].v, b1[i] = B[i].v, b2[i] = B[i].v;
  }
  trans(a0), trans(a1), trans(a2), trans(b0), trans(b1), trans(b2);
  for (int i = 0; i < size_fft; i++) {
    c0[i] = a0[i] * b0[i];
    c1[i] = a1[i] * b1[i];
    c2[i] = a2[i] * b2[i];
  }
  trans(c0, true), trans(c1, true), trans(c2, true);
  Z mod0 = MODs[0], mod01 = mod0 * MODs[1];
  std::vector<Z> res(N + M - 1);
  for (int i = 0; i < N + M - 1; i++) {
    int y0 = c0[i].v;
    int y1 = (imod0 * (c1[i] - y0)).v;
    int y2 = (imod01 * (c2[i] - y0) - imod1 * y1).v;
    res[i] = mod01 * y2 + mod0 * y1 + y0;
  }
  return res;
}
}; // namespace NTT

using Z = Mint<i64>;
struct Combination {
  std::vector<Z> fac, ifac;
  int N;
  Combination(int _N) : N(2 * _N), fac(2 * _N + 1), ifac(2 * _N + 1) {
    fac[0] = Z(1);
    for (int i = 1; i <= N; i++) {
      fac[i] = fac[i - 1] * Z(i);
    }
    ifac[N] = fac[N].inv();
    for (int i = N - 1; i >= 0; i--) {
      ifac[i] = ifac[i + 1] * Z(i + 1);
    }
  }

  Z C(int n, int k) {
    if (n < k or n < 0 or k < 0) {
      return Z(0);
    }
    return fac[n] * ifac[n - k] * ifac[k];
  }
  Z P(int n, int k) {
    if (n < k or n < 0 or k < 0) {
      return Z(0);
    }
    return fac[n] * ifac[n - k];
  }
  Z H(int n, int k) { return C(n + k - 1, n); }
  Z S(int n, int k) {
    Z ans = 0;
    for (i64 i = 0; i <= k; i++) {
      if ((k - i) % 2 == 0) {
        ans += C(k, i) * Z(i).pow(n);
      } else {
        ans -= C(k, i) * Z(i).pow(n);
      }
    }
    return ans * ifac[k];
  }
};

void solve() {
  int N, M;
  std::cin >> N >> M;
  std::vector<Z> a(N), b(M);

  for (int i = 0; i < N; i++) {
    i64 x;
    std::cin >> x;
    a[i] = x;
  }
  for (int i = 0; i < M; i++) {
    i64 x;
    std::cin >> x;
    b[i] = x;
  }
  auto c = NTT::mul(a, b);
  for (int i = 0; i < (int)c.size(); i++) {
    std::cout << c[i] << " \n"[i == (int)c.size() - 1];
  }
}
int main() {
  std::cin.tie(nullptr)->sync_with_stdio(false);
  solve();
}