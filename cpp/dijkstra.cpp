#include <bits/stdc++.h>
#include <bits/extc++.h>

template <typename T>
struct Dijkstra
{
    struct Edge
    {
        int to;
        T cost;
        Edge(int _to, T _cost) : to(_to), cost(_cost) {}
        bool operator<(const Edge &o) const
        {
            return cost > o.cost;
        }
    };
    std::vector<std::vector<Edge>> G;
    std::vector<T> ds;
    std::vector<int> bs;
    Dijkstra(int n) : G(n) {}

    void add_edge(int u, int v, T c)
    {
        G[u].emplace_back(v, c);
    }
    void build(int s)
    {
        int n = G.size();
        ds.assign(n, std::numeric_limits<T>::max());
        bs.assign(n, -1);
        std::priority_queue<Edge> pq;
        ds[s] = 0;
        pq.emplace(s, ds[s]);
        while (not pq.empty())
        {
            auto [u, cost_u] = pq.top();
            pq.pop();
            if (ds[u] < cost_u)
            {
                continue;
            }
            for (auto [v, cost_v] : G[u])
            {
                if (ds[v] > ds[u] + cost_v)
                {
                    ds[v] = ds[u] + cost_v;
                    bs[v] = u;
                    pq.emplace(v, ds[v]);
                }
            }
        }
    }

    T operator[](int k)
    {
        return ds[k];
    }
    std::vector<int> restore(int to)
    {
        std::vector<int> ans;
        if (bs[to] < 0)
        {
            return ans;
        }
        while (~to)
        {
            ans.emplace_back(to);
            to = bs[to];
        }
        std::reverse(ans.begin(), ans.end());
        return ans;
    }
};
using i64 = long long;
void solve()
{
    int n, m, s, t;
    std::cin >> n >> m >> s >> t;
    Dijkstra<i64> G(n);
    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        std::cin >> a >> b >> c;
        G.add_edge(a, b, c);
    }
    G.build(s);
    auto ps = G.restore(t);
    if (ps.empty())
    {
        std::cout << "-1\n";
        return;
    }
    int y = (int)ps.size() - 1;
    std::cout << G[t] << ' ' << y << '\n';
    for (int i = 0; i < y; i++)
    {
        std::cout << ps[i] << ' ' << ps[i + 1] << '\n';
    }
}
int main()
{
    std::cin.tie(nullptr)->sync_with_stdio(false);
    solve();
    return 0;
}