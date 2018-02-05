#include<bits/stdc++.h>

using namespace std;

struct dsu_save{
	int v, rnkv, u, rnku;
	dsu_save(){}
	dsu_save(int _v, int _rnkv, int _u, int _rnku):v(_v),rnkv(_rnkv),u(_u),rnku(_rnku){}
};

struct dsu_with_rollbacks{
    vector<int> p, rnk;
	int comps;
	stack<dsu_save> op;
    dsu_with_rollbacks(){}
    dsu_with_rollbacks(int n){p.resize(n); rnk.resize(n); for(int i = 0; i < n; i++){p[i] = i; rnk[i] = 0;} comps = n;}
    int find_set(int v){
        return (v == p[v]) ? v : find_set(p[v]);
    }
    bool unite(int v, int u){
        v = find_set(v); u = find_set(u);
        if(v == u)return false;
		comps--;
        if(rnk[v] > rnk[u])swap(v, u);
		op.push(dsu_save(v, rnk[v], u, rnk[u]));
        p[v] = u;
        if(rnk[u] == rnk[v])rnk[u]++;
        return true;
    }
	void rollback(){
		if(op.empty())return;
		dsu_save x = op.top();
		op.pop();
		comps++;
		p[x.v] = x.v;
		rnk[x.v] = x.rnkv;
		p[x.u] = x.u;
		rnk[x.u] = x.rnku;
	}
};

struct query{
	int v, u;
	bool united;
	query(int _v, int _u):v(_v),u(_u){}
};

struct QueryTree{
	vector<vector<query> > t;
	dsu_with_rollbacks dsu;
	int T;
	QueryTree(){}
	QueryTree(int _T, int n):T(_T){dsu = dsu_with_rollbacks(n); t.resize(4*T+4);}
	void add_to_tree(int v, int l, int r, int ul, int ur, query & q){
		if(ul > ur)return;
		if(l == ul && r == ur){
			t[v].push_back(q);
			return;
		}
		int mid = (l + r)/2;
		add_to_tree(2*v, l, mid, ul, min(ur, mid), q);
		add_to_tree(2*v + 1, mid + 1, r, max(ul, mid + 1), ur, q);
	}
	void add_query(query q, int l, int r){
		add_to_tree(1, 0, T-1, l, r, q);
	}
	void dfs(int v, int l, int r, vector<int> & ans){
		for(query & q : t[v]){
			q.united = dsu.unite(q.v, q.u);
		}
		if(l == r)ans[l] = dsu.comps;
		else{
			int mid = (l + r)/2;
			dfs(2*v, l, mid, ans);
			dfs(2*v + 1, mid + 1, r, ans);
		}
		for(query q : t[v]){
			if(q.united)dsu.rollback();
		}
	}
	vector<int> solve(){
		vector<int> ans(T);
		dfs(1, 0, T-1, ans);
		return ans;
	}
};

int main(){
//	freopen("connect.in", "r", stdin);
//	freopen("connect.out", "w", stdout);
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	int n, q;
	scanf("%d%d", &n, &q);
	int times = 1;
	vector<pair<char, query> > qs;
	for(int i = 0; i < q; i++){
		char c;
		do{
			c = getc(stdin);
		}while(c != '+' && c != '-' && c != '?');
		if(c == '?')qs.push_back({c, query(-1, -1)});
		else{
			times++;
			int v, u;
			scanf("%d%d", &v, &u);
			v--; u--;
			qs.push_back({c, query(v, u)});
		}
	}
	QueryTree qtree(times, n);
	map<pair<int, int>, int> lad;
	int ct = 0;
	for(int i = 0; i < q; i++){
		if(qs[i].first == '?')continue;
		if(qs[i].second.v > qs[i].second.u)swap(qs[i].second.v, qs[i].second.u);
		if(qs[i].first == '+')lad[make_pair(qs[i].second.v, qs[i].second.u)] = ++ct;
		else{
			auto x = make_pair(qs[i].second.v, qs[i].second.u);
			int l = lad[x];
			lad[x] = 0;
			int r = ++ct;
			qtree.add_query(qs[i].second, l, r - 1);
		}
	}
	for(auto x : lad)if(x.second != 0)qtree.add_query(query(x.first.first, x.first.second), x.second, ct);
	auto ans = qtree.solve();
	ct = 0;
	for(int i = 0; i < q; i++){
		if(qs[i].first == '?')printf("%d\n", ans[ct]);
		else ++ct;
	}
	return 0;
}
