#include <functional>
template <typename T>
T first_true(T l, T r, std::function<bool(T)> &&f) //[l,r]
{
    r++;
    while (l < r)
    {
        T mid = l + (r - l) / 2;
        if (f(mid))
        {
            r = mid;
        }
        else
        {
            l = mid + 1;
        }
    }
    return l;
}

template <typename T>
T last_true(T l, T r, std::function<bool(T)> &&f) //[l,r]
{
    l--;
    while (l < r)
    {
        T mid = l + (r - l + 1) / 2;
        if (f(mid))
        {
            l = mid;
        }
        else
        {
            r = mid - 1;
        }
    }
    return l;
}
