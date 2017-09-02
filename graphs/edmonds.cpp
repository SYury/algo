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

template<int T>
struct edmonds{
	int n;
	int match[T];
	int q[T];
	vector<int> gr[T];
	int base[T];
	int p[T];
	int qt, qh;
	bool inq[T];
	bool lca_used[T];
	bool blossom[T];
	edmonds(){}
	void init(int _n, vector<pair<int, int> > & e){	
		n = _n;
		I(c, e){gr[c.X].pb(c.Y); gr[c.Y].pb(c.X);}
	}
	int lca(int v, int u){
		F(i, 0, n)lca_used[i] = false;
		while(true){
			v = base[v];
			lca_used[v] = true;
			if(match[v] == -1)brk;
			v = p[match[v]];
		}
		while(true){
			u = base[u];
			if(lca_used[u])ret u;
			u = p[match[u]];
		}
		ret -1; //should never happen
	}
	void blossom_path(int v, int b, int nxt){
		while(base[v] != b){
			blossom[base[v]] = blossom[base[match[v]]] = true;
			p[v] = nxt;
			nxt = match[v];
			v = p[match[v]];
		}
	}
	int find_path(int start){
		qt = qh = 0;
		q[qt++] = start;
		inq[start] = true;
		while(qh < qt){
			int v = q[qh++];
			I(u, gr[v]){
				if(base[v] == base[u] || match[v] == u)cont;
				if(u == start || (match[u] != -1 && p[match[u]] != -1)){
					int b = lca(v, u);
					F(i, 0, n)blossom[i] = false;
					blossom_path(v, b, u);
					blossom_path(u, b, v);
					F(i, 0, n){
						if(!blossom[base[i]])cont;
						base[i] = b;
						if(!inq[i]){inq[i] = true; q[qt++] = i;}
					}
				}
				else if(p[u] == -1){
					p[u] = v;
					if(match[u] == -1)ret u;
					if(match[u] != -1){inq[match[u]] = true; q[qt++] = match[u];}
				}
			}
		}
		ret -1;
	}
	void solve(){
		F(i, 0, n)match[i] = -1;
		F(i, 0, n){
			if(match[i] == -1){
				F(i, 0, n){p[i] = -1; inq[i] = false;}
				F(j, 0, n)base[j] = j;
				int j = find_path(i);
				while(j != -1){
					int prv = p[j], up = match[prv];
					match[j] = prv; match[prv] = j;
					j = up;
				}
			}
		}
	}
};

edmonds<228> solver;
int n;
vector<pair<int, int> > e;
bool can[228][228];
set<pair<int, int> > kek;

int main(){
//	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d", &n);
	while(true){
		int v, u;
		scanf("%d", &v);
		if(feof(stdin))brk;
		scanf("%d", &u);
		v--; u--;
		can[v][u] = can[u][v] = true;
	}
	F(i, 0, n)
		F(j, i + 1, n)if(can[i][j])e.pb(mk(i, j));
	solver.init(n, e);
	solver.solve();
	kek.clear();
	F(i, 0, n){
		if(solver.match[i] == -1 || solver.match[i] < i)cont;
		kek.ins(mk(i, solver.match[i]));
	}
	printf("%d\n", 2 * (int)kek.size());
	I(c, kek)printf("%d %d\n", c.X + 1, c.Y + 1);
	ret 0;
}
