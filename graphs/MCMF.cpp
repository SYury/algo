#include<bits/stdc++.h>

//#pragma GCC optimize("O3")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx") 

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

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
		gr.resize(n);
		pot.resize(n);
		p.resize(n);
		dist.resize(n);
		fb_inq.resize(n);
	}
	void add_edge(edge e){
		gr[e.v].push_back(e);
	}
	void ford_bellman(){
		pot.assign(n, inf);
		pot[0] = 0;
		fb_inq.assign(n, false);
		fb_inq[0] = true; fb_q.push(0);
		while(!fb_q.empty()){
			int v = fb_q.front();
			fb_q.pop();
			fb_inq[v] = false;
			for(auto e : gr[v]){
				if(e.cap - e.flow == 0)continue;
				if(pot[e.u] > pot[v] + e.cost){
					pot[e.u] = pot[v] + e.cost;
					if(!fb_inq[e.u]){fb_inq[e.u] = true; fb_q.push(e.u);}
				}
			}
		}
	}
	void dijkstra(){
		dist.assign(n, inf);
		p.assign(n, -1);
		dist[0] = 0;
		s.insert(make_pair(0, 0));
		while(!s.empty()){
			int v = s.begin()->second;
			s.erase(s.begin());
			for(auto e : gr[v]){
				if(e.cap - e.flow == 0)continue;
				lint w = pot[e.u] - pot[v] + e.cost;
				if(dist[e.u] <= dist[v] + w)continue;
				p[e.u] = v;
				s.erase(make_pair(dist[e.u], e.u));
				dist[e.u] = dist[v] + w;
				s.insert(make_pair(dist[e.u], e.u));
			}
		}
		for(int i = 1; i < n; i++)if(p[i] != -1)dist[i] = dist[i] - pot[i] + pot[0];
		pot.swap(dist);
	}
	lint min_cost(int val){
		ford_bellman();
		for(int i = 0; i < val; i++){
			dijkstra();
			if(pot[n - 1] == inf)return -1;
			int v = n - 1;
			while(true){
				int pv = p[v];
				if(pv == -1)break;
				for(auto & e : gr[pv])if(e.u == v)e.flow++;
				for(auto & e : gr[v])if(e.u == pv)e.flow--;
				v = pv;
			}
		}
		lint ans = 0;
		for(int i = 0; i < n; i++)
			for(auto e : gr[i])if(e.cap != 0)ans += e.flow * 1ll * e.cost;
		return ans;
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
	for(int i = 0; i < k; i++)scanf("%d%d", &s[i], &c[i]);
	for(int i = 0; i < n; i++){
		solver.add_edge(edge(0, i + 1, 0, 1));
		solver.add_edge(edge(i + 1, 0, 0, 0));
		int t;
		scanf("%d", &t);
		for(int j = 0; j < t; j++){
			int x;
			scanf("%d", &x);
			solver.add_edge(edge(i + 1, n + x, c[x - 1], 1));
			solver.add_edge(edge(n + x, i + 1, -c[x - 1], 0));
		}
	}
	for(int i = 0; i < k; i++){
		solver.add_edge(edge(n + 1 + i, n + 1 + k, 0, s[i]));
		solver.add_edge(edge(n + 1 + k, n + 1 + i, 0, 0));
	}
	printf("%lld", solver.min_cost(n));
	return 0;
}

