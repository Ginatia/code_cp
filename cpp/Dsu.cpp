#include <vector>
#include <numeric>
#include <algorithm>
struct Dsu
{
    int n;
    std::vector<int> f, sz;
    Dsu(int _n) : n(_n)
    {
        f.resize(n);
        sz.assign(n, 1);
        std::iota(f.begin(), f.end(), 0);
    }
    int leader(int x)
    {
        if (f[x] == x)
        {
            return x;
        }
        return f[x] = leader(f[x]);
    }
    bool merge(int u, int v)
    {
        u = leader(u), v = leader(v);
        if (u == v)
        {
            return false;
        }
        if (sz[u] < sz[v])
        {
            std::swap(u, v);
        }
        f[v] = u;
        sz[u] += sz[v];
        return true;
    }
    bool same(int u, int v)
    {
        return leader(u) == leader(v);
    }
    int size(int x)
    {
        return sz[leader(x)];
    }
};