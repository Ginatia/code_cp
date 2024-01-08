class Dsu:
    def __init__(self, n: int) -> None:
        self.f = list(range(n))
        self.sz = [1]*n

    def leader(self, x: int) -> int:
        x_copy = x
        while x != self.f[x]:
            x = self.f[x]
        while x_copy != x:
            self.f[x_copy], x_copy = x, self.f[x_copy]
        return x

    def same(self, u: int, v: int) -> bool:
        return self.leader(u) == self.leader(v)

    def merge(self, u: int, v: int) -> bool:
        u, v = self.leader(u), self.leader(v)
        if u == v:
            return False
        self.f[v] = u
        self.sz[u] += self.sz[v]
        return True
