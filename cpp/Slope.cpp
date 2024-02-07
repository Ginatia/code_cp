template <typename T>
struct Slope
{
    const T INF = std::numeric_limits<T>::max() / 3;

    T min_f;
    std::priority_queue<T, std::vector<T>, std::less<>> L;
    std::priority_queue<T, std::vector<T>, std::greater<>> R;
    T add_L, add_R;

private:
    void push_R(const T &a)
    {
        R.push(a - add_R);
    }
    T top_R() const
    {
        if (R.empty())
        {
            return INF;
        }
        else
        {
            return R.top() + add_R;
        }
    }
    T pop_R()
    {
        T val = top_R();
        if (not R.empty())
        {
            R.pop();
        }
        return val;
    }

    void push_L(const T &a)
    {
        L.push(a - add_L);
    }
    T top_L() const
    {
        if (L.empty())
        {
            return INF;
        }
        else
        {
            return L.top() + add_L;
        }
    }
    T pop_L()
    {
        T val = top_L();
        if (not L.empty())
        {
            L.pop();
        }
        return val;
    }
    size_t size()
    {
        return L.size() + R.size();
    }
    T relu(T x)
    {
        return std::max<T>(0, x);
    }

public:
    Slope() : min_f(0), add_L(0), add_R(0) {}
    // L,R,min_f
    using Q = std::tuple<T, T, T>;

    Q qry() const
    {
        return Q{top_L(), top_R(), min_f};
    }
    // f(x)+=a
    void add_all(const T &a)
    {
        min_f += a;
    }
    // add \_
    // f(x)+=max(a-x,0)
    void add_a_minus_x(const T &a)
    {
        min_f += relu(a - top_R());
        push_R(a);
        push_L(pop_R());
    }
    // add _/
    // f(x)+=max(x-a,0)
    void add_x_minus_a(const T &a)
    {
        min_f += relu(top_L() - a);
        push_L(a);
        push_R(pop_L());
    }
    // add \/
    // f(x)+=|x-a|
    void add_abs(const T &a)
    {
        add_x_minus_a(a);
        add_a_minus_x(a);
    }

    // \/ -> \_
    // f_{new}(x) = min f(y)(y<=x)
    void clear_R()
    {
        while (not R.empty())
        {
            R.pop();
        }
    }

    // \/ -> _/
    // f_{new}(x) = min f(y)(y>=x)
    void clear_L()
    {
        while (not L.empty())
        {
            L.pop();
        }
    }

    // \/ -> \____/
    // f_{new}(x) = min f(y)(x-b<=y<=x-a)
    void shift(const T &a, constT &b)
    {
        assert(a <= b);
        add_L += a;
        add_R += b;
    }

    // \/. -> .\/
    // f_{new}(x) = f(x-a)
    void shift(constT &a)
    {
        shift(a, a);
    }

    T get_val(const T &x)
    {
        T ans = min_f;
        while (not L.empty())
        {
            ans += relu(pop_L() - x);
        }
        while (not R.empty())
        {
            ans += relu(x - pop_R());
        }
        return ans;
    }
    void merge(Slope &s)
    {
        if (s.size() > size())
        {
            std::swap(s.L, L);
            std::swap(s.R, R);
            std::swap(s.add_L, add_L);
            std::swap(s.add_R, add_R);
            std::swap(s.min_f, min_f);
        }
        while (not s.R.empty())
        {
            add_x_minus_a(s.pop_R());
        }
        while (not s.L.empty())
        {
            add_a_minus_x(s.pop_L());
        }
        min_f += s.min_f;
    }
};