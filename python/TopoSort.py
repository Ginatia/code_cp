from collections import deque
import heapq


class TopologicalSort:
    def __init__(self, n: int) -> None:
        self.G = [[] for _ in range(n)]
        self.indeg = [0]*n

    def add_edge(self, s: int, t: int):
        self.G[s].append(t)
        self.indeg[t] += 1

    def build(self):
        n = len(self.G)
        q = deque()
        # q=[] heapq
        used = [False]*n

        def push(u):
            if used[u]:
                return
            q.append(u)
            # heapq.heappush(q, u)
            used[u] = True

        for i in range(n):
            if self.indeg[i] == 0:
                push(i)

        ps = []
        while q:
            u = q.popleft()
            # u = heapq.heappop(q)
            ps.append(u)
            for v in self.G[u]:
                self.indeg[v] -= 1
                if self.indeg[v] == 0:
                    push(v)
        if n != len(ps):
            return []
        return ps
