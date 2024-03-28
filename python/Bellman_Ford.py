from typing import *

Edges = List[Tuple[int, int, int]]


def Bellman_Ford(es: Edges, V: int, S: int):
    INF = 10**9
    dis = [INF]*V
    dis[S] = 0
    for _ in range(V-1):
        for u, v, w in es:
            if dis[u] == INF:
                continue
            dis[v] = min(dis[v], dis[u]+w)
    for u, v, w in es:
        if dis[u] == INF:
            continue
        if dis[u]+w < dis[v]:
            return []
    return dis
