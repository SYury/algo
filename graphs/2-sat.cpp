struct sat2{
	vector<vector<int> > gr, grt;
	vector<int> comp, order;
	int n;
	int ord_ptr;
	sat2(){}
	void init(int _n){
		n = 2*_n; ord_ptr = 0;
		comp.resize(n); order.resize(n);
		gr.resize(n); grt.resize(n);
		for(int i = 0; i < n; i++){
			comp[i] = 0;
		}
	}
	void add_or(int v, int u){
		gr[v^1].push_back(u);
		gr[u^1].push_back(v);
		grt[u].push_back(v^1);
		grt[v].push_back(u^1);
	}
	void dfs(int v){
		comp[v] = -1;
		for(auto u : gr[v])
			if(comp[u] == 0)
				dfs(u);
		order[ord_ptr++] = v;
	}
	void dfs1(int v, int curr){
		comp[v] = curr;
		for(auto u : grt[v])
			if(comp[u] == -1)
				dfs1(u, curr);
	}
	bool solve(vector<char> & ans){
		for(int i = 0; i < n; i++)
			if(comp[i] == 0)
				dfs(i);
		reverse(order.begin(), order.end());
		int ptr = 0;
		for(int i = 0; i < n; i++)
			if(comp[order[i]] == -1)
				dfs1(order[i], ptr++);
		for(int i = 0; i < n; i += 2){
			if(comp[i] == comp[i + 1])
				return 0;
			ans[i] = comp[i] < comp[i + 1];
		}
		return 1;
	}
};
