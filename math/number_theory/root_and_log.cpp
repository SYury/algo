#include<bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define DF(i, l, r) for(auto i = l; i != r; i--)
#define I(i, a) for(auto i : a)
#define pb push_back
#define rs resize
#define mp make_pair
#define asg assign
#define all(x) x.begin(),x.end()
#define ret return
#define cont continue
#define brk break
#define ins insert
#define era erase
#define fi0(x) memset(x, 0, sizeof(x))
#define finf(x) memset(x, 127, sizeof(x))
#define y1 adjf
#define tm dhgdg

ll binpow(ll a, int n, ll mod){
	ll ans = 1;
	while(n){
		if(n&1){
			ans = ans * a % mod;
		}
		a = a * a % mod;
		n >>= 1;
	}
	ret ans;
}

ll divs[(int)1e5 + 5];
int ptr = 0;

ll primitive_root(ll mod){
	ptr = 0;
	ll bishkek = mod - 1;
	for(ll i = 2; i * i <= mod - 1; i++){
		if(bishkek%i != 0)cont;
		divs[ptr++] = i;
		while(bishkek%i==0)bishkek /= i;
	}
	if(bishkek > 1)divs[ptr++] = bishkek;
	F(i, 2, mod){
		bool good = true;
		F(j, 0, ptr){
			ll pw = (mod-1)/divs[j];
			ll g = binpow(i, pw, mod);
			if(g == 1){good = false; brk;}
		}
		if(good)ret i;
	}
	ret 1;
}

ll discrete_log(ll a, ll b, ll m){
	ll n = (ll) sqrt (m + .0) + 1;
	map<ll,ll> vals;
	for (ll i=n; i>=1; --i)
		vals[ binpow (a, i * n, m) ] = i;
	for (ll i=0; i<=n; ++i) {
		ll cur = (binpow (a, i, m) * b) % m;
		if (vals.count(cur)) {
			ll ans = vals[cur] * n - i;
			if (ans < m)
				return ans;
		}
	}
	return -1;
}

ll discrete_root(ll power, ll a, ll mod){
	ll g = primitive_root(mod);
	ll gg = binpow(g, power, mod);
	ll lg = discrete_log(gg, a, mod);
	if(lg == -1)ret -1;
	return binpow(g, lg, mod);
}

