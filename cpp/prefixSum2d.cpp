template <typename T>
struct prefix_array2D
{
    int n, m;
    std::vector<std::vector<T>> d;
    prefix_array2D(int _n, int _m) : n(_n), m(_m), d(n + 1, std::vector<T>(m + 1, T(0))) {}
    void add(int lx, int ly, int rx, int ry, T val)
    {
        d[lx][ly] += val;
        d[lx][ry] -= val;
        d[rx][ly] -= val;
        d[rx][ry] += val;
    }
    void build()
    {
        for (int i = 0; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                d[i][j] += d[i][j - 1];
            }
        }
        for (int i = 1; i <= n; i++)
        {
            for (int j = 0; j <= m; j++)
            {
                d[i][j] += d[i - 1][j];
            }
        }
    }
    T qry(int lx, int ly, int rx, int ry)
    {
        return d[rx][ry] - d[lx][ry] - d[rx][ly] + d[lx][ly];
    }
    std::vector<T> &operator[](int i)
    {
        return this->d[i];
    }
};