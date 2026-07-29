#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include <memory>
#include <bit>
#include <limits>

int topbit(int x) {
	if (x == 0) {
		return -1;
	}
	return 31 - std::countl_zero(static_cast<unsigned int>(x));
}

template<typename Monoid> struct SqrtTree {
	int N;
	std::vector<Monoid>A;
	std::vector<std::vector<Monoid>>prefix,suffix,between;
	
	std::vector<int>sz_log;
	std::vector<int>bit_to_lv;

	SqrtTree(const std::vector<Monoid>& A)
		:A(A) {
		N = A.size();

		int K = 0;
		while ((1 << K) < N) {
			++K;
		}
		bit_to_lv.assign(K,-1);
		while (K > 1) {
			for (int i = K / 2; i < K; i++) {
				bit_to_lv[i] = sz_log.size();
			}
			sz_log.emplace_back(K);
			K /= 2;
		}
		for (int lv = 0; lv < static_cast<int>(sz_log.size()); lv++) {
			build(lv);
		}
	}
	void build(int lv) {
		int K1 = sz_log[lv];
		int K2 = K1 / 2;
		int mask = (1<<K2) - 1;
		//prefix,suffix
		auto P = A;
		auto S = A;
		for (int i = 0; i < N; i++) {
			if (i & mask) {
				P[i] = P[i - 1] + A[i];
			}
		}
		for (int i = N - 1; i >= 0; i--) {
			if (i & mask) {
				S[i - 1] = A[i - 1] + S[i];
			}
		}
		prefix.emplace_back(P);
		suffix.emplace_back(S);

		int n = N >> K2;
		std::vector<Monoid> blk_prod(n);
		for (int i = 0; i < n; i++) {
			blk_prod[i] = S[i << K2];
		}

		std::vector<Monoid>B;

		for (int j = 0; j < n; j++) {
			int s = j >> (K1 - K2) << (K1 - K2);
			B.emplace_back(blk_prod[j]);
			for (int i = j - 1; i >= s; i--) {
				B.emplace_back(blk_prod[i] + B.back());
			}
		}
		between.emplace_back(B);
	}

	// prod of [L,R)
	Monoid prod(int L, int R) {
		--R;
		
		if (L == R) {
			return A[L];
		}
		int lv = bit_to_lv[topbit(L ^ R)];
		if (lv == -1) {
			return A[L] + A[R];
		}

		int K1 = sz_log[lv],K2 = K1 / 2;
		Monoid ans = suffix[lv][L];
		int a = 1 + (L >> K2);
		int b = (R >> K2) - 1;
		int m = 1 << (K1 - K2);
		if (a <= b) {
			int k = a >> (K1 - K2);
			a -= k * m;
			b -= k * m;
			int idx = k * (m * (m + 1) / 2);
			idx += (b + 1) * b / 2;
			idx += b - a;
		
			ans = ans + between[lv][idx];
		}
		
		ans = ans + prefix[lv][R];
		return ans;
	}
};

struct Monoid {
	int x;
	int M;
	Monoid(int _x,int _m)
		:x(_x)
		,M(_m)
	{
		x = (x % M + M) % M;
	}
	Monoid() {
		x = M = 1;
	}

	static Monoid id(int _m) {
		return Monoid(1,_m);
	}
	friend Monoid operator+(Monoid L, Monoid R) {
		long long res = (static_cast<long long>(L.x) * R.x) % L.M;
		return Monoid(static_cast<int>(res),L.M);
	}
	Monoid& operator=(int val) {
        x = (val % M + M) % M;
        return *this;
    }
};
