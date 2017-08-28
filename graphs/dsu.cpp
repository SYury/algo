#include<bits/stdc++.h>

//#pragma GCC optimize("O3")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx") 

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

struct dsu{
    vector<int> p, rnk;
    dsu(){}
    dsu(int n){p.rs(n); rnk.rs(n); F(i, 0, n){p[i] = i; rnk[i] = 0;}}
    int find_set(int v){
        ret (v == p[v]) ? v : p[v] = find_set(p[v]);
    }
    bool unite(int v, int u){
        v = find_set(v); u = find_set(u);
        if(v == u)ret false;
        if(rnk[v] > rnk[u])swap(v, u);
        p[v] = u;
        if(rnk[u] == rnk[v])rnk[u]++;
        ret true;
    }
};
