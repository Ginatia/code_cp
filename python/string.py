def z_func(s: str) -> list:
    n = len(s)
    z = [0]*(n)
    l, r = 0, 0
    for i in range(1, n):
        if i < r:
            z[i] = min(r-i, z[i-l])
        while i+z[i] < n and s[z[i]] == s[i+z[i]]:
            z[i] += 1
        if i+z[i] > r:
            l, r = i, i+z[i]
    return z


def find_position(t: str, p: str) -> list:
    """
    t:text string,p:pattern string
    from t find p
    """
    idx = []
    Lt, Lp = len(t), len(p)
    s = p+"#"+t
    z = z_func(s)
    for i in range(Lt):
        k = z[i+Lp+1]
        if k == Lp:
            idx.append(i)
    return idx


def prefix_func(s: str) -> list:
    n = len(s)
    pi = [0]*n
    for i in range(1, n):
        j = pi[i-1]
        while j > 0 and s[i] != s[j]:
            j = pi[j-1]
        if s[i] == s[j]:
            j += 1
        pi[i] = j
    return pi


def match(t: str, p: str) -> bool:
    """
    t:text string,p:pattern string
    from t find p
    """
    lps = prefix_func(p+'#'+t)
    Lt, Lp = len(t), len(p)
    for i in range(Lp+1, Lt+Lp+1):
        if lps[i] == Lp:
            return True
    return False
