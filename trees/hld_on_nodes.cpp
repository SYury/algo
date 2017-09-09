#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

const int MAXN = 1e5 + 5;
int n, q;
vector<int> gr[MAXN];
int sz[MAXN];
int pr[MAXN];
int path[MAXN];
int start[MAXN];
int pcnt = 0;
int cid = 0;
int big[MAXN];
int vid[MAXN];
int val[MAXN];
int h[MAXN];
int t[4 * MAXN];

void upd(int v, int l, int r, int pos, int val){
	if(l == r){
		t[v] = val;
		return;
	}
	int mid = (l + r) >> 1;
	if(pos <= mid)upd(2*v, l, mid, pos, val);
	else upd(2*v + 1, mid + 1, r, pos, val);
	t[v] = max(t[2*v], t[2*v + 1]);
}

int get(int v, int l, int r, int ul, int ur){
	if(ul > ur)return 0;
	if(l == ul && r == ur)return t[v];
	int mid = (l + r) >> 1;
	return max(get(2*v, l, mid, ul, min(ur, mid)), get(2*v + 1, mid + 1, r, max(ul, mid + 1), ur));
}

void calc(int v, int p = -1){
	pr[v] = p;
	h[v] = (p == -1) ? 0 : (1+h[p]);
	sz[v] = 1; big[v] = -1;
	for(auto u : gr[v]){
		if(u == p)continue;
		calc(u, v);
		sz[v] += sz[u];
	}
	int cut = sz[v]/2;
	if(sz[v]&1)cut++;
	for(auto u : gr[v]){
		if(u == p)continue;
		if(sz[u] >= cut)big[v] = u;
	}
}

void build(int v, int cpath, int p){
	vid[v] = cid++;
	path[v] = cpath;
	if(p == -1 || path[p] != path[v])start[path[v]] = v;
	upd(1, 0, n - 1, vid[v], 0);
	if(big[v] != -1){
		build(big[v], cpath, v);
	}
	for(auto u : gr[v]){
		if(u == p || u == big[v])continue;
		build(u, pcnt++, v);
	}
}

int query(int v, int u){
	int ans = 0;
	while(true){
		if(path[v] == path[u]){
			ans = max(ans, get(1, 0, n - 1, min(vid[v], vid[u]), max(vid[v], vid[u])));
			break;
		}
		if(h[start[path[v]]] < h[start[path[u]]])swap(v, u);
		ans = max(ans, get(1, 0, n - 1, vid[start[path[v]]], vid[v]));
		v = pr[start[path[v]]];
	}
	return ans;
}

void upd_vertex(int v, int add){
	val[v] += add;
	upd(1, 0, n - 1, vid[v], val[v]);
}

int main(){
	scanf("%d", &n);
	for(int i = 0; i < n - 1; i++){
		int v, u;
		scanf("%d%d", &v, &u);
		v--; u--;
		gr[v].push_back(u); gr[u].push_back(v);
	}
	calc(0);
	build(0, pcnt++, -1);
	scanf("%d", &q);
	for(int i = 0; i < q; i++){
		char tp;
	   	int	v, u;
		tp = getc(stdin);
		while(!isalpha(tp))tp = getc(stdin);
		scanf("%d%d", &v, &u);
		v--;
		if(tp == 'I'){upd_vertex(v, u); continue;}
		u--;
		printf("%d\n", query(v, u));
	}
	return 0;
}
