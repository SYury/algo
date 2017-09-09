#include<bits/stdc++.h>

//#pragma GCC optimize("O3")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx") 

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

struct dsu{
    vector<int> p, rnk;
    dsu(){}
    dsu(int n){p.resize(n); rnk.resize(n); for(int i = 0; i < n; i++){p[i] = i; rnk[i] = 0;}}
    int find_set(int v){
        return (v == p[v]) ? v : p[v] = find_set(p[v]);
    }
    bool unite(int v, int u){
        v = find_set(v); u = find_set(u);
        if(v == u)return false;
        if(rnk[v] > rnk[u])swap(v, u);
        p[v] = u;
        if(rnk[u] == rnk[v])rnk[u]++;
        return true;
    }
};
