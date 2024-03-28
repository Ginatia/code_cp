template <typename T = int>
class Edge
{
public:
    int from, to;
    T cost;
    Edge(int f, int t, T c) : from(f), to(t), cost(c) {}
};

template <typename T>
using Edges = std::vector<Edge<T>>;

template <typename T>
std::vector<T> Bellman_Ford(const Edges<T> &es, int V, int S)
{
    const T INF = std::numeric_limits<T>::max();
    std::vector dis(V, INF);
    dis[S] = 0;
    for (int i = 0; i < V - 1; i++)
    {
        for (auto &e : es)
        {
            if (dis[e.from] == INF)
                continue;
            dis[e.to] = std::min(dis[e.to], dis[e.from] + e.cost);
        }
    }
    for (auto &e : es)
    {
        if (dis[e.from] == INF)
            continue;
        if (dis[e.from] + e.cost < dis[e.to])
        {
            return std::vector<T>();
        }
    }
    return dis;
}