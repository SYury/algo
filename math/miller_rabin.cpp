#include<bits/stdc++.h>

#pragma GCC optimize("O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx") 

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

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
	return ans;
}

int testp(lint m){
	lint t = m - 1;
	int s = 0;
	while(!(t&1ll)){
		s++;
		t >>= 1;
	}
	mt19937_64 gen(time(NULL));
	for(int g = 0; g < TRIES; g++){
		lint a = gen()%(m - 2) + 2;
		if(m%a == 0)return 0;
		lint x = pw(a, t, m);
		if(x == 1){continue;}
		bool cp = false;
		for(int i = 0; i < s; i++){
			if(i > 0)x = ((__int128)x) * x % m;
			if(x == 1)return 0;
			if(x == m - 1){cp = true; break;}
		}
		if(!cp)return 0;
	}
	return 1;
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int ans = 0;
	for(int i = 2; i < 10001; i++){
		ans += testp(2 * i * 1ll * i - 1);
	}
	printf("%d", ans);
	return 0;
}
