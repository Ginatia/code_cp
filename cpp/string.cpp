#include <vector>
#include <string>
namespace internal
{
    std::vector<int> z_func(const std::string &s)
    {
        int n = s.size();
        std::vector<int> z(n);
        int l = 0, r = 0;
        for (int i = 1; i < n; i++)
        {
            if (i < r)
            {
                z[i] = std::min(r - i, z[i - l]);
            }
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            {
                z[i]++;
            }
            if (i + z[i] > r)
            {
                l = i;
                r = i + z[i];
            }
        }
        return z;
    }
    std::vector<int> find_occur(const std::string &t, const std::string &p)
    {
        std::vector<int> idx;
        int len_p = p.size(), len_t = t.size();
        std::string s = p + "#" + t;
        auto z = z_func(s);
        for (int i = 0; i < len_t; i++)
        {
            int k = z[i + len_p + 1];
            if (k == len_p)
            {
                idx.emplace_back(i);
            }
        }
        return idx;
    }

    std::vector<int> prefix_func(const std::string &s)
    {
        int n = s.size();
        std::vector<int> pi(n);
        for (int i = 1; i < n; i++)
        {
            int j = pi[i - 1];
            while (j > 0 && s[i] != s[j])
            {
                j = pi[j - 1];
            }
            if (s[i] == s[j])
            {
                j++;
            }
            pi[i] = j;
        }
        return pi;
    }

    bool match(const std::string &t, const std::string &p)
    {
        auto lps = prefix_func(p + "#" + t);
        int sz1 = t.size(), sz2 = p.size();
        for (int i = sz2 + 1; i <= sz1 + sz2; i++)
        {
            if (lps[i] == sz2)
            {
                return true;
            }
        }
        return false;
    }
}