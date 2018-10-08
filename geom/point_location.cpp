
namespace PointLocation{
	struct Edge{
		pt l, r;
	};
	inline int sgn(const ll & x){
		return le(x, 0) ? eq(x, 0) ? 0 : -1 : 1;
	}
	inline bool seg_cmp(const pt & a, const pt & b, const pt & c, const pt & d){
		int val = sgn(a.cross(b, c)) + sgn(a.cross(b, d));
		if(val != 0)
			return val > 0;
		val = sgn(c.cross(d, a)) + sgn(c.cross(d, b));
		return val < 0;
	}
	enum class QType{
		del = 2,
		add = 3,
		get = 1,
		vert = 0
	};
	struct Query{
		QType type;
		int pos;
		bool operator < (const Query & q)const{
			return (int)type < (int)q.type;
		}
	};
	vector<Edge*> solve(vector<Edge*> planar, vector<pt> queries){
		vector<Edge*> ans(queries.size(), nullptr);
		auto s = set<decltype(pt::x), std::function<bool(const decltype(pt::x)&, const decltype(pt::x)&)> >(lt);
		for(pt p : queries)
			s.insert(p.x);
		for(Edge *e : planar){
			s.insert(e->l.x);
			s.insert(e->r.x);
		}
		int cid = 0;
		auto id = map<decltype(pt::x), int, std::function<bool(const decltype(pt::x)&, const decltype(pt::x)&)> >(lt);
		for(auto x : s)
			id[x] = cid++;
		auto t = set<Edge*, std::function<bool(const Edge*, const Edge*)> >([](const Edge* l, const Edge* r){return seg_cmp(l->l, l->r, r->l, r->r);});
		auto vert = set<pair<decltype(pt::x), int>, std::function<bool(const pair<decltype(pt::x), int>&, const pair<decltype(pt::x), int>&)> >([](const pair<decltype(pt::x), int> & l, const pair<decltype(pt::x), int> & r){if(!eq(l.first, r.first))return lt(l.first, r.first); return l.second < r.second;});
		vector<vector<Query> > qs(cid);
		for(int i = 0; i < (int)queries.size(); i++){
			int x = id[queries[i].x];
			qs[x].push_back(Query{QType::get, i});
		}
		for(int i = 0; i < (int)planar.size(); i++){
			int lx = id[planar[i]->l.x], rx = id[planar[i]->r.x];
			if(lx > rx){swap(lx, rx); swap(planar[i]->l, planar[i]->r);}
			if(lx == rx){
				qs[lx].push_back(Query{QType::vert, i});
			}
			else{
				qs[lx].push_back(Query{QType::add, i});
				qs[rx].push_back(Query{QType::del, i});
			}
		}
		for(int x = 0; x < cid; x++){
			sort(qs[x].begin(), qs[x].end());
			vert.clear();
			for(Query q : qs[x]){
				if(q.type == QType::del){
					t.erase(planar[q.pos]);
				}
				if(q.type == QType::vert){
					vert.insert(make_pair(min(planar[q.pos]->l.y, planar[q.pos]->r.y), q.pos));
				}
				if(q.type == QType::add){
					t.insert(planar[q.pos]);
				}
				if(q.type == QType::get){
					auto jt = vert.upper_bound(make_pair(queries[q.pos].y, planar.size()));
					if(jt != vert.begin()){
						--jt;
						int i = jt->second;
						if(ge(max(planar[i]->l.y, planar[i]->r.y), queries[q.pos].y)){
							ans[q.pos] = planar[i];
							continue;
						}
					}
					Edge* e = new Edge;
					e->l = e->r = queries[q.pos];
					auto it = t.upper_bound(e);
					if(it == t.begin())
						ans[q.pos] = nullptr;
					else
						ans[q.pos] = *(--it);
					delete e;
				}
			}
			for(Query q : qs[x]){
				if(q.type != QType::get)
					continue;
				if(ans[q.pos] != nullptr && eq(ans[q.pos]->l.x, ans[q.pos]->r.x))
					continue;
				Edge* e = new Edge;
				e->l = e->r = queries[q.pos];
				auto it = t.upper_bound(e);
				delete e;
				if(it == t.begin())
					e = nullptr;
				else
					e = *(--it);
				if(ans[q.pos] == nullptr){ans[q.pos] = e; continue;}
				if(e == nullptr)continue;
				if(e == ans[q.pos])continue;
				if(id[ans[q.pos]->r.x] == x){
					if(id[e->l.x] == x){
						if(gt(e->l.y, ans[q.pos]->r.y))
							ans[q.pos] = e;
					}
				}
				else ans[q.pos] = e;
			}
		}
		return ans;
	}
}
