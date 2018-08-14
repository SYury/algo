#include<bits/stdc++.h>

using namespace std;

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
		for(int i = 0; i < n; i++)gr[i].clear();
		for(auto c : e){gr[c.first].push_back(c.second); gr[c.second].push_back(c.first);}
	}
	int lca(int v, int u){
		for(int i = 0; i < n; i++)lca_used[i] = false;
		while(true){
			v = base[v];
			lca_used[v] = true;
			if(match[v] == -1)break;
			v = p[match[v]];
		}
		while(true){
			u = base[u];
			if(lca_used[u])return u;
			u = p[match[u]];
		}
		return -1; //should never happen
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
			for(auto u : gr[v]){
				if(base[v] == base[u] || match[v] == u)continue;
				if(u == start || (match[u] != -1 && p[match[u]] != -1)){
					int b = lca(v, u);
					for(int i = 0; i < n; i++)blossom[i] = false;
					blossom_path(v, b, u);
					blossom_path(u, b, v);
					for(int i = 0; i < n; i++){
						if(!blossom[base[i]])continue;
						base[i] = b;
						if(!inq[i]){inq[i] = true; q[qt++] = i;}
					}
				}
				else if(p[u] == -1){
					p[u] = v;
					if(match[u] == -1)return u;
					if(match[u] != -1){inq[match[u]] = true; q[qt++] = match[u];}
				}
			}
		}
		return -1;
	}
	void solve(){
		fill(match, match + n, -1);
		for(int i = 0; i < n; i++){
			if(match[i] == -1){
				for(int j = 0; j < n; j++){p[j] = -1; inq[j] = false;}
				for(int j = 0; j < n; j++)base[j] = j;
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
