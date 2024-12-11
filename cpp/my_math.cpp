#include <vector>

/// @brief 筛素数
/// @param N<=1e7
/// @return 
/// 0:判断[0,N]的数是否是素数,
/// 1:返回[0,N]的所有数的最小素因子,
/// 2:返回[0,N]的所有素数
std::tuple<std::vector<bool>,std::vector<int>,std::vector<int>> get_prime(int N){
    std::vector<bool>is_prime(N+1,true);
    std::vector<int>min_factor(N+1,-1);
    std::vector<int>prime;

    is_prime[0]=is_prime[1]=false;
    min_factor[1]=1;

    for(int i=2;i<=N;i++){
        if(!is_prime[i]){
            continue;
        }
        min_factor[i]=i;
        prime.push_back(i);
        for(int j=i+i;j<=N;j+=i){
            is_prime[j]=false;
            if(min_factor[j]==-1){
                min_factor[j]=i;
            }
        }
    }
    return std::make_tuple(is_prime,min_factor,prime);
}