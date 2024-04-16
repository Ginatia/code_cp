unsigned int bit_ceil(unsigned int n) {
  unsigned int x = 1;
  while (x < (unsigned int)(n))
    x *= 2;
  return x;
}
int countr_zero(unsigned int n) {
  int x = 0;
  while (!(n & (1 << x)))
    x++;
  return x;
}
 
template <typename S> struct Segtree {
public:
  Segtree() : Segtree(0) {}
  explicit Segtree(int n) : Segtree(std::vector<S>(n, S())) {}
  explicit Segtree(const std::vector<S> &v) : _n(static_cast<int>(v.size())) {
    size = bit_ceil(static_cast<unsigned int>(_n));
    log = countr_zero(static_cast<unsigned int>(size));
    d = std::vector<S>(2 * size, S());
    for (int i = 0; i < _n; i++) {
      d[i + size] = v[i];
    }
    for (int i = size - 1; i >= 1; i--) {
      update(i);
    }
  }
  void set(int p,S x){
    p+=size;
    d[p]=x;
    for(int i=1;i<=log;i++){
      update(p>>i);
    }
  }
  S get(int p)const{
    return d[p+size];
  }
  S prod(int l,int r){
    S sl=S(),sr=S();
    l+=size;
    r+=size;
    while(l<r){
      if(l&1){
        sl=sl+d[l++];
      }
      if(r&1){
        sr=sr+d[--r];
      }
      l>>=1;
      r>>=1;
    }
    return sl+sr;
  }
  S all_prod()const{
    return d[1];
  }
 
private:
  int _n, size, log;
  std::vector<S> d;
 
  void update(int k) { d[k] = d[2 * k]+d[2 * k + 1]; }
};
struct S{
  i64 x;
};
S operator+(S l,S r){
  return S{l.x+r.x};
}
