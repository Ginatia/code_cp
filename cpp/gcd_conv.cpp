const int N = 1e6 + 1e3;
std::vector<int> prime;
std::vector<bool> is_prime(N + 1, true);
std::vector<int> mobius(N + 1, 1);

void sieve() {
  is_prime[0] = is_prime[1] = false;
  for (int p = 2; p <= N; p++) {
    if (!is_prime[p])
      continue;
    prime.push_back(p);
    mobius[p] = -1;
    for (int q = 2 * p; q <= N; q += p) {
      is_prime[q] = false;
      if ((q / p) % p == 0)
        mobius[q] = 0;
      else
        mobius[q] = -mobius[q];
    }
  }
}

template <typename T> std::vector<T> fast_zeta(std::vector<T> F) {
  int n = F.size();
  for (int p = 2; p < n; p++) {
    if (!is_prime[p])
      continue;
    for (int k = (n - 1) / p; k >= 1; k--) {
      F[k] += F[k * p];
    }
  }
  return F;
}

template <typename T> std::vector<T> fast_mobius(std::vector<T> f) {
  int n = f.size();
  for (int p = 2; p < n; p++) {
    if (!is_prime[p])
      continue;
    for (int k = 1; k * p < n; k++) {
      f[k] -= f[k * p];
    }
  }
  return f;
}

template <typename T>
std::vector<T> gcd_conv(const std::vector<T> &f, const std::vector<T> &g) {
  int n = std::max(f.size(), g.size());
  auto F = fast_zeta(f);
  auto G = fast_zeta(g);
  std::vector<T> H(n);
  for (int i = 1; i < n; i++)
    H[i] = F[i] * G[i];
  return fast_mobius(H);
}