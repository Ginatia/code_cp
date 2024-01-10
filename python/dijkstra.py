import heapq


class Dijkstra:

    def __init__(self, _n: int) -> None:
        self.G = [[] for _ in range(_n)]
        self.bs = [-1]*_n

    def add_edge(self, u: int, v: int, c):
        self.G[u].append((v, c))

    def build(self, s: int, INF):
        n = len(self.G)
        ds = [INF]*n
        ds[s] = 0
        pq = []
        heapq.heappush(pq, (0, s))
        while len(pq) > 0:
            cost_u, u = heapq.heappop(pq)
            if ds[u] < cost_u:
                continue
            for v, cost_v in self.G[u]:
                if ds[v] > ds[u]+cost_v:
                    ds[v] = ds[u]+cost_v
                    self.bs[v] = u
                    heapq.heappush(pq, (ds[v], v))
        return ds

    def restore(self, to: int):
        ans = []
        if self.bs[to] < 0:
            return ans
        while ~to:
            ans.append(to)
            to = self.bs[to]
        return ans[::-1]
