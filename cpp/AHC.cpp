#include <chrono>
#include <cmath>
#include <random>
class TimeKeeperDouble{
public:
    std::chrono::high_resolution_clock::time_point start_time;
    double TL,now_time{0};

    TimeKeeperDouble(const double _TL):start_time(std::chrono::high_resolution_clock::now()),TL(_TL){}
    void setNowTime(){
        auto diff=std::chrono::high_resolution_clock::now()-start_time;
        this->now_time=std::chrono::duration_cast<std::chrono::microseconds>(diff).count()*1e-3;
    }
    double getNowTime()const{
        return this->now_time;
    }
    bool isTimeOver()const{
        return now_time>=TL;
    }
};

class Random{
public:
    std::mt19937 mt;
    std::uniform_real_distribution<double>d{0,1.0};
    Random(const int seed=0):mt(std::mt19937(seed)){}

    int random_range(int l,int r){
        std::uniform_int_distribution<int>dis(l,r);
        return dis(mt);
    }
    int next_int(int m){
        std::uniform_int_distribution<int>dis(0,m-1);
        return dis(mt);
    }
    double next_double(){
        return d(mt);
    }
    double next_log(){
        return std::log(d(mt));
    }
};