// Mint
template <typename T, T MOD = 1000000007>
struct Mint
{
    inline static constexpr T mod = MOD;
    T v;
    Mint() : v(0) {}
    Mint(signed v) : v(v) {}
    Mint(long long t)
    {
        v = t % MOD;
        if (v < 0)
            v += MOD;
    }

    Mint pow(long long k)
    {
        Mint res(1), tmp(v);
        while (k)
        {
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

    Mint &operator+=(Mint a)
    {
        v += a.v;
        if (v >= MOD)
            v -= MOD;
        return *this;
    }
    Mint &operator-=(Mint a)
    {
        v += MOD - a.v;
        if (v >= MOD)
            v -= MOD;
        return *this;
    }
    Mint &operator*=(Mint a)
    {
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

    static Mint comb(long long n, int k)
    {
        Mint num(1), dom(1);
        for (int i = 0; i < k; i++)
        {
            num *= Mint(n - i);
            dom *= Mint(i + 1);
        }
        return num / dom;
    }
};
template <typename T, T MOD>
std::ostream &operator<<(std::ostream &os, Mint<T, MOD> m)
{
    os << m.v;
    return os;
}

// NTT
constexpr int bmds(int x)
{
    const int v[] = {1012924417, 924844033, 998244353,
                     897581057, 645922817};
    return v[x];
}
constexpr int brts(int x)
{
    const int v[] = {5, 5, 3, 3, 3};
    return v[x];
}

template <int X>
struct NTT
{
    inline static constexpr int md = bmds(X);
    inline static constexpr int rt = brts(X);
    using M = Mint<int, md>;
    std::vector<std::vector<M>> rts, rrts;

    void ensure_base(int n)
    {
        if ((int)rts.size() >= n)
            return;
        rts.resize(n);
        rrts.resize(n);
        for (int i = 1; i < n; i <<= 1)
        {
            if (!rts[i].empty())
                continue;
            M w = M(rt).pow((md - 1) / (i << 1));
            M rw = w.inv();
            rts[i].resize(i);
            rrts[i].resize(i);
            rts[i][0] = M(1);
            rrts[i][0] = M(1);
            for (int k = 1; k < i; k++)
            {
                rts[i][k] = rts[i][k - 1] * w;
                rrts[i][k] = rrts[i][k - 1] * rw;
            }
        }
    }

    void ntt(std::vector<M> &as, bool f)
    {
        int n = as.size();
        assert((n & (n - 1)) == 0);
        ensure_base(n);

        for (int i = 0, j = 1; j + 1 < n; j++)
        {
            for (int k = n >> 1; k > (i ^= k); k >>= 1)
                ;
            if (i > j)
                std::swap(as[i], as[j]);
        }

        for (int i = 1; i < n; i <<= 1)
        {
            for (int j = 0; j < n; j += i * 2)
            {
                for (int k = 0; k < i; k++)
                {
                    M z = as[i + j + k] * (f ? rrts[i][k] : rts[i][k]);
                    as[i + j + k] = as[j + k] - z;
                    as[j + k] += z;
                }
            }
        }

        if (f)
        {
            M tmp = M(n).inv();
            for (int i = 0; i < n; i++)
                as[i] *= tmp;
        }
    }

    std::vector<M> multiply(std::vector<M> as, std::vector<M> bs)
    {
        int need = as.size() + bs.size() - 1;
        int sz = 1;
        while (sz < need)
            sz <<= 1;
        as.resize(sz, M(0));
        bs.resize(sz, M(0));

        ntt(as, 0);
        ntt(bs, 0);
        for (int i = 0; i < sz; i++)
            as[i] *= bs[i];
        ntt(as, 1);

        as.resize(need);
        return as;
    }

    std::vector<int> multiply(std::vector<int> as, std::vector<int> bs)
    {
        std::vector<M> am(as.size()), bm(bs.size());
        for (int i = 0; i < (int)am.size(); i++)
            am[i] = M(as[i]);
        for (int i = 0; i < (int)bm.size(); i++)
            bm[i] = M(bs[i]);
        std::vector<M> cm = multiply(am, bm);
        std::vector<int> cs(cm.size());
        for (int i = 0; i < (int)cs.size(); i++)
            cs[i] = cm[i].v;
        return cs;
    }
};

// Garner
struct Garner
{
    using ll = long long;
    inline static NTT<0> ntt0;
    inline static NTT<1> ntt1;
    inline static NTT<2> ntt2;

    static constexpr int pow(int a, int b, int md)
    {
        int res = 1;
        a = a % md;
        while (b)
        {
            if (b & 1)
                res = (ll)res * a % md;
            a = (ll)a * a % md;
            b >>= 1;
        }
        return res;
    }

    static constexpr int inv(int x, int md)
    {
        return pow(x, md - 2, md);
    }

    inline void garner(int &c0, int c1, int c2, int m01, int MOD)
    {
        static constexpr int r01 = inv(ntt0.md, ntt1.md);
        static constexpr int r02 = inv(ntt0.md, ntt2.md);
        static constexpr int r12 = inv(ntt1.md, ntt2.md);

        c1 = (ll)(c1 - c0) * r01 % ntt1.md;
        if (c1 < 0)
            c1 += ntt1.md;

        c2 = (ll)(c2 - c0) * r02 % ntt2.md;
        c2 = (ll)(c2 - c1) * r12 % ntt2.md;
        if (c2 < 0)
            c2 += ntt2.md;

        c0 %= MOD;
        c0 += (ll)c1 * ntt0.md % MOD;
        if (c0 >= MOD)
            c0 -= MOD;
        c0 += (ll)c2 * m01 % MOD;
        if (c0 >= MOD)
            c0 -= MOD;
    }

    inline void garner(std::vector<std::vector<int>> &cs, int MOD)
    {
        int m01 = (ll)ntt0.md * ntt1.md % MOD;
        int sz = cs[0].size();
        for (int i = 0; i < sz; i++)
            garner(cs[0][i], cs[1][i], cs[2][i], m01, MOD);
    }

    std::vector<int> multiply(std::vector<int> as, std::vector<int> bs, int MOD)
    {
        std::vector<std::vector<int>> cs(3);
        cs[0] = ntt0.multiply(as, bs);
        cs[1] = ntt1.multiply(as, bs);
        cs[2] = ntt2.multiply(as, bs);
        size_t sz = as.size() + bs.size() - 1;
        for (auto &v : cs)
            v.resize(sz);
        garner(cs, MOD);
        return cs[0];
    }

    template <typename T>
    decltype(auto) multiply(std::vector<T> am,
                            std::vector<T> bm)
    {
        std::vector<int> as(am.size()), bs(bm.size());
        for (int i = 0; i < (int)as.size(); i++)
            as[i] = am[i].v;
        for (int i = 0; i < (int)bs.size(); i++)
            bs[i] = bm[i].v;
        std::vector<int> cs = multiply(as, bs, T::mod);
        std::vector<T> cm(cs.size());
        for (int i = 0; i < (int)cm.size(); i++)
            cm[i] = T(cs[i]);
        return cm;
    }
};