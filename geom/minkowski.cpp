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
#define y1 adjf

const ldb eps = 1e-9;

struct pt{
	int x, y;
	pt(){}
	pt(int px, int py):x(px), y(py){}
	void get(){scanf("%d%d", &x, &y);}
};

pt operator + (const pt & a, const pt & b){
	ret pt(a.x + b.x, a.y + b.y);
}

pt operator - (const pt & a, const pt & b){
	ret pt(a.x - b.x, a.y - b.y);
}

lint operator * (const pt & a, const pt & b){
	ret a.x * 1ll * b.x + a.y * 1ll * b.y;
}

lint ar(pt a, pt b){
	ret a.x * 1ll * b.y - a.y * 1ll * b.x;	
}

bool rseg(pt a, pt b, pt c){
	ret ((a - b) * (c - b) >= 0) && ((b - a) * (c - a) >= 0);
}

lint rng(pt a, pt b){
	ret (a.x - b.x) * 1ll * (a.x - b.x) + (a.y - b.y) * 1ll * (a.y - b.y);
}

ldb rngl(pt a, pt b, pt c){
	lint la = b.y - a.y;
	lint lb = a.x - b.x;
	lint lc = - la * a.x - lb * a.y;
	ret ((la * c.x + lb * c.y + lc) / ldb(la * la + lb * lb)) * (la * c.x + lb * c.y + lc);
}

int n, m;
vector<pt> a, b;
vector<pt> c;


int main(){
//	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d%d", &n, &m);
	a.rs(n); b.rs(m);
	F(i, 0, n)a[i].get();
	F(j, 0, m)b[j].get();
	F(j, 0, m)b[j].x = -b[j].x, b[j].y = -b[j].y;
	int li = 0, lj = 0;
	F(i, 1, n){
		if(a[i].y < a[li].y || (a[i].y == a[li].y && a[i].x < a[li].x))li = i;
	}
	F(j, 1, m){
		if(b[j].y < b[lj].y || (b[j].y == b[lj].y && b[j].x < b[lj].x))lj = j;
	}
	int i = 0, j = 0;
	rotate(a.begin(), a.begin() + li, a.end());
	rotate(b.begin(), b.begin() + lj, b.end());
	a.pb(a[0]); a.pb(a[1]);
	b.pb(b[0]); b.pb(b[1]);
	while(i < n || j < m){
		c.pb(a[i] + b[j]);
		if(ar(a[i + 1] - a[i], b[j + 1] - b[j]) > 0)i++;
		else if(ar(a[i + 1] - a[i], b[j + 1] - b[j]) < 0)j++;
		else {i++; j++;}
	}
	int t = c.size();
	//F(i, 0, t)printf("c[%d] = (%d, %d)\n", i, c[i].x, c[i].y);
	c.pb(c[0]);
	lint sar = 0;
	lint tar = 0;
	pt z(0, 0);
	F(i, 0, t){
		sar += llabs(ar(c[i], c[i + 1]));
		tar += ar(c[i], c[i + 1]);
	}
	if(sar != llabs(tar)){printf("0"); ret 0;}
	ldb ans = 1e18;
	F(i, 0, t){
		ldb curr;
		if(!rseg(c[i], c[i + 1], z))curr = min(rng(c[i], z), rng(c[i + 1], z));
		else curr = rngl(c[i], c[i + 1], z);
		if(ans - curr > eps)ans = curr;
	}
	ans = sqrt(ans);
	if(ans - 60 > eps){printf("%.15lf", (double)(ans - 60));}
	else printf("0");
	ret 0;
}
