#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

const int MAXN = 1e5 + 5;
vector<int> gr[MAXN];
bool col[MAXN];
bool used[MAXN];
set<pair<int, int> > white[MAXN];
map<int, int> dist_to_root[MAXN];
int pr[MAXN];
int sz[MAXN];
int n, q;

void calc(int v, int p, int root, int cu){
	sz[v] = 1;
	dist_to_root[v][root] = cu;
	for(auto u : gr[v]){
		if(u == p || used[u])continue;
		calc(u, v, root, cu + 1);
		sz[v] += sz[u];
	}
}

void build(int v, int p = -1){
	calc(v, -1, p, 1);
	int half = sz[v]>>1;
	if(sz[v]&1)half++;
	int prv = -1;
	while(true){
		int msz = 0, mx = -1;
		for(auto u : gr[v]){
			if(u == prv || used[u])continue;
			if(sz[u] > msz){msz = sz[u]; mx = u;}
		}
		prv = v;
		if(mx == -1 || msz <= half)break;
		else v = mx;
	}
	if(v == -1)return;
	used[v] = true;
	dist_to_root[v][v] = 0;
	pr[v] = p;
	for(auto u : gr[v]){
		if(used[u])continue;
		build(u, v);
	}
}

int main(){
	scanf("%d", &n);
	for(int i = 0; i < n - 1; i++){
		int v, u;
		scanf("%d%d", &v, &u);
		v--; u--;
		gr[v].push_back(u); gr[u].push_back(v);
	}
	for(int i = 0; i < n; i++)col[i] = false;
	build(0, -1);
	scanf("%d", &q);
	for(int i = 0; i < q; i++){
		int tp, v;
		scanf("%d%d", &tp, &v);
		v--;
		if(tp == 0){
			col[v] = !col[v];
			int u = v;
			while(v != -1){
				if(col[u]){
					white[v].insert(make_pair(dist_to_root[u][v], u));
				}
				else{
					white[v].erase(make_pair(dist_to_root[u][v], u));
				}
				v = pr[v];
			}
		}
		else{
			int add = 0;
			int ans = MAXN;
			int u = v;
			while(v != -1){
				add = dist_to_root[u][v];
				if(!white[v].empty()){
					ans = min(ans, white[v].begin()->first + add);
				}
				v = pr[v];
			}
			if(ans == MAXN)ans = -1;
			printf("%d\n", ans);
		}
	}
	return 0;
}
