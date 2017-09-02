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
		F(i, 0, n)gr[i].clear();
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
int n, r;
pair<int, int> c[33];
bool can[33][33];
pair<int, int> d[33];
int p[33];
int deg[33];
int ans = 33;
int msk = 0;
edmonds<33> msc2_solver;

int dist(pair<int, int> l, pair<int, int> r){
	ret (l.X - r.X) * (l.X - r.X) + (l.Y - r.Y) * (l.Y - r.Y);
}

struct cset{
	int mask;
	int id;
	bool has(int i){ret mask&(1<<i);}
};

vector<int> empty_vector(0);

vector<int> msc2(vector<cset> s, int covered, bool & can){
	int old = covered;
	F(i, 0, (int)s.size())covered |= s[i].mask;
	if(covered != (1<<n)-1){can = false; ret empty_vector;}
	can = true;
	vector<pair<int, int> > e;
	I(c, s){
		int l = -1, r;
		F(i, 0, n)if(c.has(i)){if(l==-1)l = i; else r = i;}
		e.pb(mk(l, r));
	}
	msc2_solver.init(n, e);
	msc2_solver.solve();
	vector<int> res;
	vector<pair<int, int> >  edges;
	F(i, 0, n){
		if(msc2_solver.match[i] > i){edges.pb(mk(i, msc2_solver.match[i]));}
		else if(msc2_solver.match[i] == -1 && !(old&(1<<i))){
			if(msc2_solver.gr[i].empty()){can = false; ret empty_vector;}
			int j = msc2_solver.gr[i][0];
			edges.pb(mk(i, j));
		}
	}
	I(c, edges){
		F(i, 0, (int)s.size()){
			if(s[i].has(c.X) && s[i].has(c.Y)){res.pb(s[i].id); brk;}
		}
	}
	ret res;
}

void outmask(int mask){F(i, 0, n)if(mask&(1<<i))putc('1', stdout); else putc('0', stdout);}

vector<cset> del(int mask, vector<cset> s){
	vector<cset> res(0);
	F(i, 0, (int)s.size()){
		//printf("Deleting "); outmask(mask); printf(" from "); outmask(s[i].mask); printf("\n");
		s[i].mask &= (~mask);
		//printf("Got "); outmask(s[i].mask); printf("\n");
		if(s[i].mask != 0)res.pb(s[i]);
	}
	ret res;
}

inline int cover(int mask, int add){
	ret mask|add;
}

vector<int> msc(vector<cset> s, int covered, bool & can){
//	printf("s = "); F(i, 0, (int)s.size())printf("%d ", s[i].id);
//	printf("mask = "); outmask(covered); printf("\n");
	if(s.size() == 0){if(covered != (1<<n)-1)can = false; else can = true; ret empty_vector;}
	F(i, 0, (int)s.size()){
		F(j, 0, (int)s.size()){
			if(i == j)cont;
			if((s[i].mask|s[j].mask)==s[j].mask){
				s.era(s.begin() + i);
				ret msc(s, covered, can);
			}
		}
	}
	F(i, 0, n){
		if(covered&(1<<i))cont;
		int cnt = 0;
		int pos = -1;
		F(j, 0, (int)s.size()){
			if(s[j].has(i)){pos = j; cnt++;}
		}
		if(cnt == 1){
			int nc = cover(covered, s[pos].mask);
			vector<int> tmp = msc(del(s[pos].mask, s), nc, can);
			tmp.pb(s[pos].id);
			ret tmp;
		}
	}
	int pos = -1, sz = 0;
	F(i, 0, (int)s.size()){
		int len = __builtin_popcount(s[i].mask);
		if(len > sz){sz = len; pos = i;}
	}
	if(sz == 2)ret msc2(s, covered, can);
	bool kek, bishkek;
	int nc0 = cover(covered, s[pos].mask);
	vector<int> res = msc(del(s[pos].mask, s), nc0, kek);
	res.pb(s[pos].id);
	s.era(s.begin() + pos);
	vector<int> res1 = msc(s, covered, bishkek);
	if(!kek){can = bishkek; ret res1;}
	if(!bishkek){can = kek; ret res;}
	can = true;
	if(res.size() < res1.size())ret res; else ret res1;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d%d", &n, &r);
	F(i, 0, n)scanf("%d%d", &c[i].X, &c[i].Y);
	//n = 30; r = 1; F(i, 0, n)c[i] = mk(0, i);
	F(i, 0, n)
		F(j, i + 1, n)if(dist(c[i], c[j]) <= r * r)can[i][j] = can[j][i] = true;
	vector<cset> s(n);
	F(i, 0, n){
		int mask = 0;
		F(j, 0, n)if(i == j || can[i][j])mask |= 1<<j;
		s[i].mask = mask; s[i].id = i + 1;
	}
	bool can;
	vector<int> bishkek = msc(s, 0, can);
	printf("%d\n", (int)bishkek.size());
	F(i, 0, (int)bishkek.size())printf("%d ", bishkek[i]);
	ret 0;
}
