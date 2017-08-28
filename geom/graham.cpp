#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define Df(i, l, r) for(auto i = l; i != r; i--)
#define uF(i, l, r) for(i = l; i != r; i++)
#define uDf(i, l, r) for(i = l; i != r; i--)
#define I(a, x) for(auto a : x)
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
#define acpy(y, x) memcpy(y, x, sizeof(y))
#define y1 adjf
#define tm fjfjfk

struct pt{
	int x, y;
	pt(){}
	pt(int _x, int _y):x(_x),y(_y){}
	void get(){
		scanf("%d%d", &x, &y);
	}
};

int operator * (const pt & a, const pt & b){
	ret a.x * b.y - b.x * a.y;
}

pt operator - (const pt & a, const pt & b){
	ret pt(a.x - b.x, a.y - b.y);
}

lint dist(const pt & a, const pt & b){
	ret (a.x - b.x) * 1ll * (a.x - b.x) + (a.y - b.y) * 1ll * (a.y - b.y);
}

struct comp{
	pt base;
	comp(){}
	comp(pt _base):base(_base){}
	bool operator () (const pt & l, const pt & r){
		ret (l - base) * (r - base) > 0 || ((l - base) * (r - base) == 0 && dist(l, base) > dist(r, base));
	}
};

typedef vector<pt> poly;

poly convex_hull(poly p){
	int n = p.size();
	if(n == 1)ret p;
	int pos = 0;
	F(i, 1, n)if(p[i].y < p[pos].y || (p[i].y == p[pos].y && p[i].x < p[pos].x))pos = i;
	swap(p[pos], p[0]);
	sort(p.begin() + 1, p.end(), comp(p[0]));
	vector<int> stk(n);
	stk[0] = 0;
	int ptr = 1;
	F(i, 1, n){
		while(ptr > 1 && (p[i] - p[stk[ptr - 2]]) * (p[stk[ptr - 1]] - p[stk[ptr - 2]]) > 0)ptr--;
		stk[ptr++] = i;
	}
	poly res(ptr);
	F(i, 0, ptr)res[i] = p[stk[i]];
	ret res;
} 
