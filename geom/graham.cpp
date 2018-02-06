#include<bits/stdc++.h>

using namespace std;
typedef long long ll;

struct pt{
	ll x, y;
	pt(){}
	pt(ll _x, ll _y):x(_x),y(_y){}
	void get(){
		scanf("%lld%lld", &x, &y);
	}
	pt operator - (const pt & a)const{return pt(x - a.x, y - a.y);}
	ll cross(const pt & a)const{return x * a.y - y * a.x;}
	ll cross(const pt & a, const pt & b)const{return (a - *this).cross(b - *this);}
	ll sqrnorm()const{return x * x + y * y;}
};

struct comp{
	pt base;
	comp(pt _base):base(_base){}
	bool operator ()(const pt & l, const pt & r)const{
		return base.cross(l, r) > 0 || (base.cross(l, r) == 0 && (l - base).sqrnorm() < (r - base).sqrnorm());
	}
};

typedef vector<pt> poly;

poly convex_hull(poly p){
	int n = p.size();
	if(n == 1)return p;
	int pos = 0;
	for(int i = 1; i < n; i++)if(p[i].y < p[pos].y || (p[i].y == p[pos].y && p[i].x < p[pos].x))pos = i;
	swap(p[pos], p[0]);
	sort(p.begin() + 1, p.end(), comp(p[0]));
	int ptr = n - 2;
	while(ptr > 0 && p[0].cross(p[ptr], p[n - 1]) == 0)ptr--;
	reverse(p.begin() + ptr + 1, p.end());
	vector<int> stk(n);
	stk[0] = 0;
	int ptr = 1;
	for(int i = 1; i < n; i++){
		while(ptr > 1 && p[stk[ptr - 2]].cross(p[i], p[stk[ptr-1]]) > 0)ptr--;
		stk[ptr++] = i;
	}
	poly res(ptr);
	for(int i = 0; i < ptr; i++)res[i] = p[stk[i]];
	return res;
} 
