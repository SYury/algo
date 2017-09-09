#include<bits/stdc++.h>

//#pragma GCC optimize("O3")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx") 

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

struct dsu_save{
	int v, rnkv, u, rnku;
	dsu_save(){}
	dsu_save(int _v, int _rnkv, int _u, int _rnku):v(_v),rnkv(_rnkv),u(_u),rnku(_rnku){}
};

struct dsu_with_rollbacks{
    vector<int> p, rnk;
	stack<dsu_save> op;
    dsu_with_rollbacks(){}
    dsu_with_rollbacks(int n){p.resize(n); rnk.resize(n); for(int i = 0; i < n; i++){p[i] = i; rnk[i] = 0;}}
    int find_set(int v){
        return (v == p[v]) ? v : find_set(p[v]);
    }
    bool unite(int v, int u){
        v = find_set(v); u = find_set(u);
        if(v == u)return false;
        if(rnk[v] > rnk[u])swap(v, u);
		op.push(dsu_save(v, rnk[v], u, rnk[u]));
        p[v] = u;
        if(rnk[u] == rnk[v])rnk[u]++;
        return true;
    }
	void rollback(){
		if(op.empty())return;
		dsu_save x = op.top();
		op.pop();
		p[x.v] = x.v;
		rnk[x.v] = x.rnkv;
		p[x.u] = x.u;
		rnk[x.u] = x.rnku;
	}
};
