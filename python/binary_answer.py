def first_true(L: int, R: int, f):
    R += 1
    while L < R:
        mid = L+(R-L)//2
        if f(mid):
            R = mid
        else:
            L = mid+1
    return L


def last_true(L: int, R: int, f):
    L -= 1
    while L < R:
        mid = L+(R-L+1)//2
        if f(mid):
            L = mid
        else:
            R = mid-1
    return L


def test():
    a = list(range(10))

    def even(i):
        return a[i] % 2 == 0

    print(first_true(0, 10, even), last_true(0, 10, even))


test()
