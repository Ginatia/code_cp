#include <vector>
#include <cassert>

namespace feishi
{
    unsigned int bit_ceil(unsigned int n)
    {
        unsigned int x = 1;
        while (x < (unsigned int)(n))
        {
            x *= 2;
        }
        return x;
    }
    int countr_zero(unsigned int n)
    {
        int x = 0;
        while (!(n & (1 << x)))
        {
            x++;
        }
        return x;
    }
    template <typename S, S (*op)(S, S), S (*e)()>
    class segtree
    {
    public:
        segtree() : segtree(0) {}
        explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
        explicit segtree(const std::vector<S> &v) : n(int(v.size()))
        {
            size = (int)bit_ceil((unsigned int)(n));
            log = countr_zero((unsigned int)size);
            d = std::vector<S>(2 * size, e());
            for (int i = 0; i < n; i++)
            {
                d[size + i] = v[i];
            }
            for (int i = size - 1; i >= 1; i--)
            {
                update(i);
            }
        }
        void set(int p, S x)
        {
            assert(0 <= p && p < n);
            p += size;
            d[p] = x;
            for (int i = 1; i <= log; i++)
            {
                update(p >> i);
            }
        }
        S get(int p) const
        {
            assert(0 <= p && p < n);
            return d[p + size];
        }
        S prod(int l, int r) const
        {
            assert(0 <= l && l <= r && r <= n);
            S sl = e(), sr = e();
            l += size;
            r += size;
            while (l < r)
            {
                if (l & 1)
                {
                    sl = op(sl, d[l++]);
                }
                if (r & 1)
                {
                    sr = op(sr, d[--r]);
                }
                l >>= 1;
                r >>= 1;
            }
            return op(sl, sr);
        }
        S all_prod() const
        {
            return d[1];
        }
        template <bool (*f)(S)>
        int max_right(int l) const
        {
            return max_right(l, [](S x)
                             { return f(x); });
        }
        template <typename F>
        int max_right(int l, F f) const
        {
            assert(0 <= l && l <= n);
            assert(f(e()));
            if (l == n)
            {
                return n;
            }
            l += size;
            S sm = e();
            do
            {
                while (l % 2 == 0)
                {
                    l >>= 1;
                }
                if (not f(op(sm, d[l])))
                {
                    while (l < size)
                    {
                        l = l << 1;
                        if (f(op(sm, d[l])))
                        {
                            sm = op(sm, d[l]);
                            l++;
                        }
                    }
                    return l - size;
                }
                sm = op(sm, d[l]);
                l++;
            } while ((l & -l) != l);
            return n;
        }
        template <bool (*f)(S)>
        int min_left(int r) const
        {
            return min_left(r, [](S x)
                            { return f(x); });
        }
        template <class F>
        int min_left(int r, F f) const
        {
            assert(0 <= r && r <= _n);
            assert(f(e()));
            if (r == 0)
                return 0;
            r += size;
            S sm = e();
            do
            {
                r--;
                while (r > 1 && (r % 2))
                    r >>= 1;
                if (!f(op(d[r], sm)))
                {
                    while (r < size)
                    {
                        r = (2 * r + 1);
                        if (f(op(d[r], sm)))
                        {
                            sm = op(d[r], sm);
                            r--;
                        }
                    }
                    return r + 1 - size;
                }
                sm = op(d[r], sm);
            } while ((r & -r) != r);
            return 0;
        }

    private:
        int n, size, log;
        std::vector<S> d;
        void update(int k)
        {
            d[k] = op(d[2 * k], d[2 * d + 1]);
        }
    };
}