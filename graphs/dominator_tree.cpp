#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(int i = l; i != r; i++)
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
#define cset fhjfghjgfh

template<int T>
struct dominator_tree{
	//0 is root
	vector<int> gr[T];
	int p[T];
	vector<int> grt[T];
	int sdom[T];
	int vid[T];
	int idom[T];
	int anc[T];
	int label[T];
	int sz[T], child[T];
	int n;
	int cid;
	vector<int> bucket[T];
	void init(int _n, vector<pair<int, int> > & e){
		n = _n;
		F(i, 0, n){gr[i].clear(); grt[i].clear(); bucket[i].clear();}
		I(c, e)gr[c.X].pb(c.Y);
	}
	void dfs(int v){
		sdom[v] = cid++;
		vid[cid - 1] = v;
		I(u, gr[v]){
			if(sdom[u] == -1){p[u] = v; dfs(u);}
			grt[u].pb(v);
		}
	}
	void compress(int v){
		if(anc[anc[v]] != -1){
			compress(anc[v]);
			if(sdom[label[anc[v]]] < sdom[label[v]])label[v] = label[anc[v]];
			anc[v] = anc[anc[v]];
		}
	}
	int eval(int v){if(anc[v] == -1)ret v; else {compress(v); ret label[v];}}
	void link(int v, int u){anc[u] = v;}
	void build(){
		cid = 0;
		F(i, 0, n){idom[i] = -1; sz[i] = 1; child[i] = -1; anc[i] = -1; label[i] = i; p[i] = -1; sdom[i] = -1;}
		dfs(0);
		swap(n, cid);
		Df(j, n - 1, 0){
			int v = vid[j];
			I(u, grt[v]){
				int w = eval(u);
				if(sdom[w] < sdom[v])sdom[v] = sdom[w];
			}
			bucket[vid[sdom[v]]].pb(v);
			link(p[v], v);
			I(u, bucket[p[v]]){
				int w = eval(u);
				idom[u] = (sdom[w] < sdom[u]) ? w : p[v];
			}
			bucket[p[v]].clear();
		}
		F(i, 1, n){
			int v = vid[i];
			if(idom[v] != vid[sdom[v]])idom[v] = idom[idom[v]];
		}
		idom[0] = -1;
	}
};

dominator_tree<200022> solver;
vector<pair<int, int> > e;
int n, m;
bool kek[200022];

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	F(g, 0, 10){
		scanf("%d%d", &n, &m);
		e.rs(m);
		F(i, 0, m){
			int v, u;
			scanf("%d%d", &v, &u);
			v--; u--;
			e[i] = mk(v, u);
		}
		solver.init(n, e);
		solver.build();
		F(i, 0, n)kek[i] = false;
		//F(i, 0, n)printf("%d ", solver.idom[i]);
		//printf("\n");
		F(i, 0, n)kek[solver.idom[i]] = true;
		int cnt = 0;
		F(i, 0, n)if(kek[i])cnt++;
		printf("%d\n", cnt);
		F(i, 0, n)if(kek[i])printf("%d ", i + 1);
		printf("\n");
	}
	ret 0;
}
