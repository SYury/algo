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

const lint inf = 1e18;

struct edge{
	int v, u;
	int cost;
	int flow = 0;
	int cap;
	edge(){}
	edge(int sv, int su, int scost, int scap):v(sv),u(su),cost(scost),cap(scap){}
};

struct MCMF{
	int n;
	vector<vector<edge> > gr;
	vector<lint> pot;
	vector<int> p;
	queue<int> fb_q;
	vector<int> fb_inq;
	vector<lint> dist;
	set<pair<lint, int> > s;
	void set_n(int sn){
		n = sn;
		gr.rs(n);
		pot.rs(n);
		p.rs(n);
		dist.rs(n);
		fb_inq.rs(n);
	}
	void add_edge(edge e){
		gr[e.v].pb(e);
	}
	void ford_bellman(){
		pot.asg(n, inf);
		pot[0] = 0;
		fb_inq.asg(n, false);
		fb_inq[0] = true; fb_q.push(0);
		while(!fb_q.empty()){
			int v = fb_q.front();
			fb_q.pop();
			fb_inq[v] = false;
			I(e, gr[v]){
				if(e.cap - e.flow == 0)cont;
				if(pot[e.u] > pot[v] + e.cost){
					pot[e.u] = pot[v] + e.cost;
					if(!fb_inq[e.u]){fb_inq[e.u] = true; fb_q.push(e.u);}
				}
			}
		}
	}
	void dijkstra(){
		dist.asg(n, inf);
		p.asg(n, -1);
		dist[0] = 0;
		s.ins(mk(0, 0));
		while(!s.empty()){
			int v = s.begin()->Y;
			s.era(s.begin());
			I(e, gr[v]){
				if(e.cap - e.flow == 0)cont;
				lint w = pot[e.u] - pot[v] + e.cost;
				if(dist[e.u] <= dist[v] + w)cont;
				p[e.u] = v;
				s.era(mk(dist[e.u], e.u));
				dist[e.u] = dist[v] + w;
				s.ins(mk(dist[e.u], e.u));
			}
		}
		F(i, 1, n)if(p[i] != -1)dist[i] = dist[i] - pot[i] + pot[0];
		pot.swap(dist);
	}
	lint min_cost(int val){
		ford_bellman();
		F(i, 0, val){
			dijkstra();
			if(pot[n - 1] == inf)ret -1;
			int v = n - 1;
			while(true){
				int pv = p[v];
				if(pv == -1)brk;
				for(auto & e : gr[pv])if(e.u == v)e.flow++;
				for(auto & e : gr[v])if(e.u == pv)e.flow--;
				v = pv;
			}
		}
		lint ans = 0;
		F(i, 0, n)
			I(e, gr[i])if(e.cap != 0)ans += e.flow * 1ll * e.cost;
		ret ans;
	}
};

MCMF solver;

const int MAXN = 1e2 + 2;

int k, n;
int s[MAXN], c[MAXN];


int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	scanf("%d%d", &k, &n);
	solver.set_n(2 + n + k);
	F(i, 0, k)scanf("%d%d", &s[i], &c[i]);
	F(i, 0, n){
		solver.add_edge(edge(0, i + 1, 0, 1));
		solver.add_edge(edge(i + 1, 0, 0, 0));
		int t;
		scanf("%d", &t);
		F(j, 0, t){
			int x;
			scanf("%d", &x);
			solver.add_edge(edge(i + 1, n + x, c[x - 1], 1));
			solver.add_edge(edge(n + x, i + 1, -c[x - 1], 0));
		}
	}
	F(i, 0, k){
		solver.add_edge(edge(n + 1 + i, n + 1 + k, 0, s[i]));
		solver.add_edge(edge(n + 1 + k, n + 1 + i, 0, 0));
	}
	printf("%lld", solver.min_cost(n));
	ret 0;
}

