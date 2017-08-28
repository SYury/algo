#include<bits/stdc++.h>

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
#define tm afhdhn

typedef vector<vector<lint> > matrix;

int mod;

matrix mul(const matrix & a, const matrix & b){
	matrix ans;
	ans.rs(a.size());
	F(i, 0, (int)ans.size()){
		ans[i].rs(b[0].size());
		F(j, 0, (int)ans[i].size()){
			ans[i][j] = 0;
			F(k, 0, (int)b.size())ans[i][j] += a[i][k] * b[k][j] % mod;
			ans[i][j] %= mod;
		}
	}
	ret ans;
}

matrix binpow(matrix & a, int n){
	matrix ans;
	bool muld = false;
	while(n){
		if(n&1){
			if(!muld){muld = true; ans = a;} else {ans = mul(ans, a);}
		}
		a = mul(a, a);
		n >>= 1;
	}
	ret ans;
}

int n, x;
matrix f;
int k[111];
int c[111];

int main(){
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d%d%d", &n, &x, &mod);
	F(i, 0, n)scanf("%d", &k[i]);
	F(i, 0, n)scanf("%d", &c[i]);
	f.rs(n);
	F(i, 0, n){
		f[i].rs(n);
		if(i < n - 1)F(j, 0, n){if(j == i + 1)f[i][j] = 1; else f[i][j] = 0;}
		else F(j, 0, n)f[i][j] = c[j];
	}
	matrix res = binpow(f, x - 1);
	matrix a(n);
	F(i, 0, n){
		a[i].rs(1); a[i][0] = k[i];
	}
	matrix lel = mul(res, a);
	printf("%lld", lel[0][0]);
	ret 0;
}

