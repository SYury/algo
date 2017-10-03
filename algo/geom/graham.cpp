#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

struct pt{
	int x, y;
	pt(){}
	pt(int _x, int _y):x(_x),y(_y){}
	void get(){
		scanf("%d%d", &x, &y);
	}
};

int operator * (const pt & a, const pt & b){
	return a.x * 1ll * b.y - b.x * 1ll * a.y;
}

pt operator - (const pt & a, const pt & b){
	return pt(a.x - b.x, a.y - b.y);
}

lint dist(const pt & a, const pt & b){
	return (a.x - b.x) * 1ll * (a.x - b.x) + (a.y - b.y) * 1ll * (a.y - b.y);
}

struct comp{
	pt base;
	comp(){}
	comp(pt _base):base(_base){}
	bool operator () (const pt & l, const pt & r){
		return (l - base) * (r - base) > 0 || ((l - base) * (r - base) == 0 && dist(l, base) > dist(r, base));
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
	vector<int> stk(n);
	stk[0] = 0;
	int ptr = 1;
	for(int i = 1; i < n; i++){
		while(ptr > 1 && (p[i] - p[stk[ptr - 2]]) * (p[stk[ptr - 1]] - p[stk[ptr - 2]]) > 0)ptr--;
		stk[ptr++] = i;
	}
	poly res(ptr);
	for(int i = 0; i < ptr; i++)res[i] = p[stk[i]];
	return res;
} 
