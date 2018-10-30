const long long flow_inf = 1e18;
struct MCMF{
	struct FlowEdge{
		int v, u;
		long long cap, cost, flow = 0;
		FlowEdge(int _v, int _u, long long _cap, long long _cost):v(_v), u(_u), cap(_cap), cost(_cost){}
	};
	vector<FlowEdge> edges;
	vector<vector<int> > adj;
	int n, m = 0;
	int s, t;
	vector<long long> pot;
	vector<int> p;
	queue<int> fb_q;
	vector<int> fb_inq;
	vector<long long> dist;
	set<pair<long long, int> > ds;
	int qh, qt;
	void resize(int _n){n = _n; adj.resize(n); pot.resize(n); p.resize(n); dist.resize(n); fb_inq.resize(n);}
	MCMF(){}
	MCMF(int _n, int _s, int _t){resize(_n); s = _s; t = _t;}
	void add_edge(int v, int u, long long cap, long long cost){
		edges.push_back(FlowEdge(v, u, cap, cost));
		edges.push_back(FlowEdge(u, v, 0, -cost));
		adj[v].push_back(m);
		adj[u].push_back(m + 1);
		m += 2;
	}
	void ford_bellman(){
		pot.assign(n, flow_inf);
		pot[s] = 0;
		fb_inq.assign(n, false);
		fb_inq[s] = true; fb_q.push(s);
		while(!fb_q.empty()){
			int v = fb_q.front();
			fb_q.pop();
			fb_inq[v] = false;
			for(int id : adj[v]){
				auto& e = edges[id];
				if(e.cap - e.flow == 0)continue;
				if(pot[e.u] > pot[v] + e.cost){
					pot[e.u] = pot[v] + e.cost;
					if(!fb_inq[e.u]){fb_inq[e.u] = true; fb_q.push(e.u);}
				}
			}
		}
	}
	void dijkstra(){
		dist.assign(n, flow_inf);
		p.assign(n, -1);
		dist[s] = 0;
		ds.insert(make_pair(s, 0));
		while(!ds.empty()){
			int v = ds.begin()->second;
			ds.erase(ds.begin());
			for(int id : adj[v]){
				auto& e = edges[id];
				if(e.cap - e.flow == 0)continue;
				long long w = pot[e.u] - pot[v] + e.cost;
				if(dist[e.u] <= dist[v] + w)continue;
				p[e.u] = id;
				ds.erase(make_pair(dist[e.u], e.u));
				dist[e.u] = dist[v] + w;
				ds.insert(make_pair(dist[e.u], e.u));
			}
		}
		for(int i = 0; i < n; i++)if(i != s && p[i] != -1)dist[i] = dist[i] - pot[i] + pot[s];
		pot.swap(dist);
	}
	long long min_cost_no_potential(long long val = flow_inf){
		long long total_flow = 0, total_cost = 0;
		for(;;){
			dijkstra();
			if(pot[t] == flow_inf)break;
			int v = t;
			long long pushed = val - total_flow;
			if(!pushed)
				break;
			while(true){
				int id = p[v];
				if(id == -1)break;
				pushed = min(pushed, edges[id].cap - edges[id].flow);
				v = edges[id].v;
			}
			total_flow += pushed;
			v = t;
			while(true){
				int id = p[v];
				if(id == -1)break;
				total_cost += edges[id].cost * pushed;
				edges[id].flow += pushed;
				edges[id^1].flow -= pushed;
				v = edges[id].v;
			}
		}
		return total_cost;
	}
	long long min_cost(long long val = flow_inf){
		ford_bellman();
		return min_cost_no_potential(val);
	}
};
