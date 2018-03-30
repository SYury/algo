template<size_t MAXN>
struct Kuhn{
	int match[MAXN];
	int used[MAXN];
	bool was[MAXN];
	vector<int> gr[MAXN];
	int n;
	int cc;
	Kuhn(){}
	Kuhn(int _n):n(_n){
		for(int i = 0; i < n; i++){
			match[i] = -1;
			used[i] = was[i] = 0;
		}
	}
	void add_edge(int v, int u){
		gr[v].push_back(u);
		gr[u].push_back(v);
	}
	bool dfs(int v){
		if(used[v] == cc)return false;
		used[v] = cc;
		for(auto u : gr[v]){
			if(match[u] == -1){
				was[v] = true;
				match[u] = v;
				return true;
			}
		}
		for(auto u : gr[v]){
			if(dfs(match[u])){
				was[v] = true;
				match[u] = v;
				return true;
			}
		}
		return false;
	}
	void solve(){
		cc = 0;
		for(int i = 0; i < n; i++){used[i] = 0; was[i] = false; match[i] = -1;}
		for(int run = 1; run;){
			cc++;
			run = 0;
			for(int i = 0; i < n; i++)used[i] = false;
			for(int i = 0; i < n; i++){
				if(!was[i] && dfs(i))run = 1;
			}
		}	
	}
};
