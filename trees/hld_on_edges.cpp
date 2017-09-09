#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

const int MAXN = 1e5 + 5;
int n, q;
vector<pair<int, int> > gr[MAXN];
int sz[MAXN];
int pr[MAXN];
int path[MAXN];
int start[MAXN];
int pcnt = 0;
int cid = 0;
int big[MAXN];
int in[MAXN];
int vid[MAXN];
int h[MAXN];
int nxt[MAXN];
int t[4 * MAXN];
pair<int, int> ed[MAXN];

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
	if(ul > ur)return -1;
	if(l == ul && r == ur)return t[v];
	int mid = (l + r) >> 1;
	return max(get(2*v, l, mid, ul, min(ur, mid)), get(2*v + 1, mid + 1, r, max(ul, mid + 1), ur));
}

void calc(int v, int p = -1){
	pr[v] = p;
	h[v] = (p == -1) ? 0 : (1+h[p]);
	sz[v] = 1; big[v] = -1;
	for(auto c : gr[v]){
		int u = c.first, w = c.second;
		if(u == p)continue;
		in[u] = w;
		calc(u, v);
		sz[v] += sz[u];
	}
	int cut = sz[v]/2;
	if(sz[v]&1)cut++;
	for(auto c : gr[v]){
		int u = c.first;
		if(u == p)continue;
		if(sz[u] >= cut)big[v] = u;
	}
}

void build(int v, int cpath, int p){
	vid[v] = cid++;
	path[v] = cpath;
	nxt[v] = -1;
	if(p == -1 || path[p] != path[v])start[path[v]] = v;
	upd(1, 0, n - 1, vid[v], in[v]);
	if(big[v] != -1){
		nxt[v] = big[v];
		build(big[v], cpath, v);
	}
	for(auto c : gr[v]){
		int u = c.first;
		if(u == p || u == big[v])continue;
		build(u, pcnt++, v);
	}
}

int query(int v, int u){
	if(v == u)return -1;
	int ans = -1;
	while(true){
		if(path[v] == path[u]){
			if(h[v] < h[u])swap(v, u);
			if(v == u){break;}
			u = nxt[u];
			ans = max(ans, get(1, 0, n - 1, min(vid[v], vid[u]), max(vid[v], vid[u])));
			break;
		}
		if(h[start[path[v]]] < h[start[path[u]]])swap(v, u);
		ans = max(ans, get(1, 0, n - 1, vid[start[path[v]]], vid[v]));
		v = pr[start[path[v]]];
	}
	return ans;
}

void upd_edge(int v, int u, int val){
	if(pr[u] == v)swap(v, u);
	upd(1, 0, n - 1, vid[v], val);
}

int main(){
	int t;
	scanf("%d", &t);
	while(t){
		in[0] = -1;
		scanf("%d", &n);
		for(int i = 0; i < n; i++)gr[i].clear();
		for(int i = 0; i < n - 1; i++){
			int v, u, c;
			scanf("%d%d%d", &v, &u, &c);
			v--; u--;
			ed[i] = make_pair(v, u);
			gr[v].push_back(make_pair(u, c)); gr[u].push_back(make_pair(v, c));
		}
		pcnt = 0; cid = 0;
		calc(0);
		build(0, pcnt++, -1);
		for(;;){
			char tp;
		   	int	v, u;
			tp = getc(stdin);
			while(!isalpha(tp))tp = getc(stdin);
			char kek = getc(stdin);
			while(isalpha(kek))kek = getc(stdin);
			if(tp == 'D')break;
			scanf("%d%d", &v, &u);
			v--;
			if(tp == 'C'){upd_edge(ed[v].first, ed[v].second, u); continue;}
			u--;
			printf("%d\n", query(v, u));
		}
		t--;
	}
	return 0;
}
