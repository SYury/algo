#include<bits/stdc++.h>

typedef int64_t ll;

//GCC only
namespace Factor{
	const int CNT_SMALL_PRIMES = 12;
	const int SMALL_PRIMES[CNT_SMALL_PRIMES] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	std::mt19937_64 random_gen(std::chrono::steady_clock::now().time_since_epoch().count());
	inline ll gcd(ll a, ll b){
		if(b == 0)
			return a;
		int shift = __builtin_ctzll(a | b);
		b >>= __builtin_ctzll(b);
		while(a){
			a >>= __builtin_ctzll(a);
			if(a < b)
				std::swap(a, b);
			a -= b;
		}
		return b<<shift;
	}
	inline ll abs(ll a){
		return a < 0 ? -a : a;
	}
	inline ll mul_mod(ll a, ll b, ll mod){
	#if defined(__LP64__) || defined(_WIN64)
		return __int128(a) * b % mod;
	#else
		ll res = 0;
		while(b){
			if(b&1){res += a; res -= res >= mod ? mod : 0;}
			a += a;
			a -= a >= mod ? mod : 0;
			b >>= 1;
		}
		return res;
		//or calculate in doubles for speed
	#endif
	}
	inline ll pw(ll a, ll to, ll mod){
		ll res = 1;
		while(to){
			if(to&1)
				res = mul_mod(res, a, mod);
			a = mul_mod(a, a, mod);
			to >>= 1;
		}
		return res;
	}
	inline bool miller_rabin(ll n, ll a){
		a %= n;
		if(a < 2 || a == n - 1)return false;
		int shift = __builtin_ctzll(n - 1);
		ll t = (n - 1)>>shift;
		ll kek = pw(a, t, n);
		if(kek == 1)
			return false;
		if(kek == n - 1)
			return false;
		for(int i = 1; i < shift; i++){
			kek = mul_mod(kek, kek, n);
			if(kek == 1)
				return true;
			if(kek == n - 1)
				return false;
		}
		return true;
	}
	inline bool test_prime(ll n){
		if(n == 1)
			return false;
		for(int i = 0; i < CNT_SMALL_PRIMES && SMALL_PRIMES[i] < n; i++){
			if(miller_rabin(n, SMALL_PRIMES[i]))
				return false;
		}
		return true;
	}
	inline ll G(ll x, ll mod){
		x = mul_mod(x, x, mod);
		++x;
		x -= x == mod ? mod : 0;
		return x;
	}
	inline void factorize(ll n, std::vector<ll> & p){
		if(n == 1)
			return;
		if(test_prime(n)){
			p.push_back(n);
			return;
		}
		for(int i = 0; i < CNT_SMALL_PRIMES; i++){
			if(n%SMALL_PRIMES[i] == 0){
				p.push_back(SMALL_PRIMES[i]);
				factorize(n/SMALL_PRIMES[i], p);
				return;
			}
		}
		while(true){
			ll x = random_gen()%(n - 1) + 1;
			ll y = G(x, n);
			int a = 1, b = 2;
			while(true){
				ll d = x - y;
				d += d < 0 ? n : 0;
				if(!d)
					break;
				ll g = gcd(d, n);
				if(g != 1){
					factorize(g, p);
					factorize(n/g, p);
					return;
				}
				if(a * 2 == b){
					a *= 2;
					x = y;
				}
				++b;
				y = G(y, n);
			}
		}
	}
}
