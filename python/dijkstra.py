import heapq


def solve():
    n, m = map(int, input().split())
    G = [[] for _ in range(n)]
    for _ in range(m):
        a, b, c = map(int, input().split())
        a -= 1
        b -= 1
        G[a].append((b, c))
        G[b].append((a, c))

    def dist(s: int):
        pq = []
        INF = 10**9
        ds = [INF]*n
        ds[s] = 0
        heapq.heappush(pq, (0, s))
        while len(pq) > 0:
            cost_u, u = heapq.heappop(pq)
            if ds[u] < cost_u:
                continue
            for v, cost_v in G[u]:
                if ds[v] > ds[u]+cost_v:
                    ds[v] = ds[u]+cost_v
                    heapq.heappush(pq, (ds[v], v))
        return ds

    d1 = dist(0)
    dn = dist(n-1)
    for i in range(n):
        print(d1[i]+dn[i])


solve()
