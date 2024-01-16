#include <vector>
#include <queue>

class TopologicalSort
{
    std::vector<std::vector<int>> G;
    std::vector<int> indeg;

public:
    TopologicalSort(int n) : G(n), indeg(n, 0) {}

    void add_edge(int s, int t)
    {
        G[s].emplace_back(t);
        indeg[t]++;
    }
    std::vector<int> build()
    {
        int n = (int)G.size();
        std::queue<int> q;
        // use prority to get lexicographical order
        std::vector<bool> used(n, false);

        auto push = [&](int u)
        {
            if (used[u])
            {
                return;
            }
            q.emplace(u);
            used[u] = true;
        };

        for (int i = 0; i < n; i++)
        {
            if (indeg[i] == 0)
            {
                push(i);
            }
        }

        std::vector<int> ps;
        while (not q.empty())
        {
            int u = q.front();
            q.pop();
            ps.emplace_back(u);
            for (int v : G[u])
            {
                if (--indeg[v] == 0)
                {
                    push(v);
                }
            }
        }
        if (n != (int)ps.size())
        {
            return {};
        }
        return ps;
    }
};