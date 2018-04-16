const long long flow_inf = 1e18;

struct Dinic{
	struct FlowEdge{
		int v, u;
		long long cap, flow = 0;
		FlowEdge(int _v, int _u, long long _cap):v(_v), u(_u), cap(_cap){}
	};
	vector<FlowEdge> edges;
	vector<vector<int> > adj;
	int n, m = 0;
	int s, t;
	vector<int> level, ptr;
	vector<int> q;
	int qh, qt;
	void resize(int _n){n = _n; q.resize(n); adj.resize(n); level.resize(n); ptr.resize(n);}
	Dinic(){}
	Dinic(int _n, int _s, int _t){resize(_n); s = _s; t = _t;}
	void add_edge(int v, int u, long long cap){
		edges.push_back(FlowEdge(v, u, cap));
		edges.push_back(FlowEdge(u, v, 0));
		adj[v].push_back(m);
		adj[u].push_back(m + 1);
		m += 2;
	}
	bool bfs(){
		while(qh < qt){
			int v = q[qh++];
			for(int id : adj[v]){
				if(edges[id].cap - edges[id].flow < 1)continue;
				if(level[edges[id].u] != -1)continue;
				level[edges[id].u] = level[v] + 1;
				q[qt++] = edges[id].u;
			}
		}
		return level[t] != -1;
	}
	long long dfs(int v, long long pushed = flow_inf){
		if(pushed == 0)return 0;
		if(v == t)return pushed;
		for(int & cid = ptr[v]; cid < (int)adj[v].size(); cid++){
			int id = adj[v][cid];
			int u = edges[id].u;
			if(level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)continue;
			long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
			if(tr == 0)continue;
			edges[id].flow += tr;
			edges[id^1].flow -= tr;
			return tr;
		}
		return 0;
	}
	long long flow(){
		long long f = 0;
		while(true){
			ptr.assign(n, 0);
			level.assign(n, -1);
			level[s] = 0;
			qh = 0, qt = 1;
			q[0] = s;
			if(!bfs())break;
			while(long long pushed = dfs(s)){f += pushed;}
		}
		return f;
	}
};

