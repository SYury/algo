#include<bits/stdc++.h>

#pragma GCC optimize("O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx") 

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define Df(i, l, r) for(auto i = l; i != r; i--)
#define I(i, a) for(auto i : a)
#define pb push_back
#define rs resize
#define mk make_pair
#define asg assign
#define all(x) x.begin(),x.end()
#define ret return
#define cont continue
#define brk break
#define ins insert
#define era erase
#define fi0(x) memset(x, 0, sizeof(x))
#define finf(x) memset(x, 127, sizeof(x))
#define acpy(y, x) memcpy(y, x, sizeof(y))
#define y1 adjf
#define tm dhgdg

const int TRIES = 25;

lint pw(lint a, lint x, lint mod){
	lint ans = 1ll;
	while(x > 0){
		if(x&1){
			ans = ((__int128)ans) * a % mod;
			x--;
		}
		else{
			a = ((__int128)a) * a % mod;
			x >>= 1;
		}
	}
	ret ans;
}

int testp(lint m){
	lint t = m - 1;
	int s = 0;
	while(!(t&1ll)){
		s++;
		t >>= 1;
	}
	mt19937_64 gen(time(NULL));
	F(g, 0, TRIES){
		lint a = gen()%(m - 2) + 2;
		if(m%a == 0)ret 0;
		lint x = pw(a, t, m);
		if(x == 1){cont;}
		bool cp = false;
		F(i, 0, s){
			if(i > 0)x = ((__int128)x) * x % m;
			if(x == 1)ret 0;
			if(x == m - 1){cp = true; brk;}
		}
		if(!cp)ret 0;
	}
	ret 1;
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int ans = 0;
	F(i, 2, 10001){
		ans += testp(2 * i * 1ll * i - 1);
	}
	printf("%d", ans);
	ret 0;
}
