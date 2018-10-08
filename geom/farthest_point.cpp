
//ADD BASIC GEOM HERE

struct Edge{
	pt origin;
	Edge * nxt = nullptr;
	Edge * prv = nullptr;
	Edge * twin = nullptr;
	int site_id = -1;
	bool alive = true;
};

inline Line get_edge(Edge* e){
	if(e->prv == nullptr)
		return Line(e->twin->origin + e->origin, e->twin->origin);
	if(e->nxt == nullptr)
		return Line(e->origin, e->origin + e->twin->origin);
	else
		return Line(e->origin, e->nxt->origin);
}

inline bool on_edge(pt p, Edge* e){
	if(e->prv == nullptr){
		return ge(e->twin->origin.dot(p, e->twin->origin + e->origin), 0);
	}
	if(e->nxt == nullptr){
		return ge(e->origin.dot(p, e->origin + e->twin->origin), 0);
	}
	else return le(p.dot(e->origin, e->twin->origin), 0);
}

struct AddedVertex{
	pt p;
	int cell;
	Edge* e;
};

inline pt trCirc(pt a, pt b, pt c){
	return interLineLine(Line((a + b)/2, (a + b)/2 + Line(a, b).getOrth()), Line((a + c)/2, (a + c)/2 + Line(a, c).getOrth()))[0];
}

void clean_prefix(Edge *start, Edge *finish, pt breakpoint, Edge *in){
	Edge *e;
	for(e = start; e != finish; e = e->prv){
		e->alive = e->twin->alive = false;
	}
	if(e->prv != nullptr && breakpoint == e->origin){
		e->alive = e->twin->alive = false;
		e = e->prv;
	}
	e->nxt = in->twin;
	in->twin->prv = e;
}

void clean_segment(Edge *start, Edge *finish, pt fbreakpoint, pt sbreakpoint, Edge *in){
	Edge *e = start->prv;
	if(start->nxt != nullptr && start->nxt->origin == fbreakpoint){
		start->alive = start->twin->alive = false;
		start->nxt->prv = in->twin;
		in->twin->nxt = start->nxt;
		e = start;
	}
	else{
		start->origin = fbreakpoint;
		start->prv = in->twin;
		in->twin->nxt = start;
	}
	for(; e != finish; e = e->prv){
		e->alive = e->twin->alive = false;
	}
	if(finish->prv != nullptr && finish->origin == sbreakpoint){
		finish->alive = finish->twin->alive =false;
		finish = finish->prv;
	}
	finish->nxt = in->twin;
	in->twin->prv = finish;
}

void clean_suffix(Edge *start, Edge *finish, pt breakpoint, Edge *in){
	Edge *e;
	for(e = start; e != finish; e = e->nxt){
		if(e->nxt != nullptr && e->nxt->origin == breakpoint)break;
		e->alive = e->twin->alive = false;
	}
	if(e->nxt != nullptr && e->nxt->origin == breakpoint){e->alive = e->twin->alive = false; e = e->nxt;}
	e->origin = breakpoint;
	e->prv = in;
	in->nxt = e;
}

vector<Edge*> farthest_point(vector<pt> p){
	int n = p.size();
	mt19937 rnd(time(0));
	vector<int> what_on(n), where_is(n);
	for(int i = 0; i < n; i++){
		what_on[i] = i;
		where_is[i] = i;
		int j = rnd()%(i + 1);
		if(j < 0)j += i + 1;
		swap(p[i], p[j]);
		swap(where_is[what_on[j]], where_is[what_on[i]]);
		swap(what_on[i], what_on[j]);
	}
	if(lt(p[0].cross(p[1], p[2]), 0)){
		swap(p[2], p[1]);
		swap(where_is[what_on[2]], where_is[what_on[1]]);
		swap(what_on[2], what_on[1]);
	}
	vector<int> cw(p.size()), ccw(p.size());
	vector<int> lp(p.size()), rp(p.size());
	for(int i = 0; i < n; i++){
		rp[i] = where_is[(what_on[i] + 1) < n ? what_on[i] + 1 : 0];
		lp[i] = where_is[(what_on[i] - 1) >= 0 ? what_on[i] - 1 : (n - 1)];
	}
	for(int i = n - 1; i >= 3; i--){
		cw[i] = lp[i];
		ccw[i] = rp[i];
		rp[lp[i]] = rp[i];
		lp[rp[i]] = lp[i];
	}
	pt cc = trCirc(p[0], p[1], p[2]);
	vector<Edge*> ans;
	vector<Edge*> most_ccw(n, nullptr);
	vector<Edge*> e01, e02, e12;
	pt d01, d02, d12;
	pt m01, m02, m12;
	Edge *fe, *se;
	Line l1, l2;
	pt v1, v2;
	d01 = Line(p[0], p[1]).getOrth();
	d02 = Line(p[0], p[2]).getOrth();
	d12 = Line(p[1], p[2]).getOrth();
	//0
	fe = new Edge; se = new Edge;
	se->origin = cc;
	most_ccw[0] = se;
	if(gt(p[1].cross(p[0], p[1] + d01), 0))d01 *= -1;
	fe->origin = d01;
	fe->nxt = se;
	se->prv = fe;
	fe->site_id = se->site_id = 0;
	ans.push_back(fe); ans.push_back(se);
	//1	
	fe = new Edge; se = new Edge;
	se->origin = cc;
	most_ccw[1] = se;
	if(gt(p[2].cross(p[1], p[2] + d12), 0))d12 *= -1;
	fe->origin = d12;
	fe->nxt = se;
	se->prv = fe;
	fe->site_id = se->site_id = 1;
	ans.push_back(fe); ans.push_back(se);
	//2
	fe = new Edge; se = new Edge;
	se->origin = cc;
	most_ccw[2] = se;
	if(gt(p[0].cross(p[2], p[0] + d02), 0))d02 *= -1;
	fe->origin = d02;
	fe->nxt = se;
	se->prv = fe;
	fe->site_id = se->site_id = 2;
	ans.push_back(fe); ans.push_back(se);
	//twins
	most_ccw[0]->twin = most_ccw[2]->prv; most_ccw[2]->prv->twin = most_ccw[0];
	most_ccw[1]->twin = most_ccw[0]->prv; most_ccw[0]->prv->twin = most_ccw[1];
	most_ccw[2]->twin = most_ccw[1]->prv; most_ccw[1]->prv->twin = most_ccw[2];
	vector<Edge*> startwith(n);
	for(int i = 3; i < n; i++){
		int cell = ccw[i];
		Edge* e = most_ccw[cell];
		Edge* last = nullptr;
		Edge* last_e = nullptr;
		Line bs = Line((p[cell] + p[i])/2.0, (p[cell] + p[i])/2.0 + Line(p[i], p[cell]).getOrth());
		pt pp;
		pt edir;
		bool was = false;
		vector<AddedVertex> v;
		startwith[ccw[i]] = e;
		while(true){
			assert(e != nullptr);
			vector<pt> kek = {trCirc(p[i], p[cell], p[e->twin->site_id])};
			if(on_edge(kek[0], e)){
				v.push_back(AddedVertex{kek[0], cell, e});
				if(e->twin->site_id == cw[i]){
					startwith[cw[i]] = e->twin;
					break;
				}
				if(e->twin->prv != nullptr && e->twin->origin == kek[0])e = e->twin->prv->prv;
				else e = e->twin->prv;
				cell = e->site_id;
				startwith[cell] = e;
				if(cell == cw[i])break;
			}
			else e = e->prv;
		}
		Edge *fe, *se, *tmp;
		fe = new Edge;
		se = new Edge;
		fe->site_id = i;
		fe->twin = se;
		se->origin = v[0].p;
		se->twin = fe;
		se->site_id = ccw[i];
		Edge *keke = most_ccw[ccw[i]];
		clean_prefix(most_ccw[ccw[i]], v[0].e, v[0].p, fe);
		last = fe;
		most_ccw[ccw[i]] = se;
		ans.push_back(fe);
		ans.push_back(se);
		for(int j = 0; j + 1 < (int)v.size(); j++){
			fe = new Edge;
			se = new Edge;
			fe->site_id = i;
			fe->twin = se;
			se->origin = v[j + 1].p;
			fe->origin = v[j].p;
			se->twin = fe;
			se->site_id = v[j + 1].cell;
			fe->prv = last;
			last->nxt = fe;
			last = fe;
			clean_segment(startwith[v[j + 1].cell]->nxt, v[j + 1].e, v[j].p, v[j + 1].p, fe);
			ans.push_back(fe);
			ans.push_back(se);
		}
		fe = new Edge; se = new Edge;
		fe->site_id = i;
		fe->twin = se;
		fe->origin = v.back().p;
		se->twin = fe;
		se->site_id = cw[i];
		last->nxt = fe;
		fe->prv = last;
		most_ccw[i] = fe;
		tmp = startwith[cw[i]];
		if(tmp->nxt != nullptr && tmp->nxt->origin == v.back().p)tmp = tmp->nxt;
		clean_suffix(keke->twin, startwith[cw[i]], v[(int)v.size() - 1].p, se);
		auto ns = se->nxt;
		pt vv = Line(p[i], p[cw[i]]).getOrth();
		pt nv = (ns->nxt == nullptr) ? ns->twin->origin : (ns->twin->origin - ns->origin);
		if(lt(vv.cross(nv), 0))vv *= -1;
		se->origin = vv * -1;
		tmp = most_ccw[ccw[i]]->twin;
		ns = tmp->nxt;
		vv = Line(p[i], p[ccw[i]]).getOrth();
		nv = (ns->nxt == nullptr) ? ns->twin->origin : (ns->twin->origin - ns->origin);
		if(lt(vv.cross(nv), 0))vv *= -1;
		tmp->origin = vv * -1;
		ans.push_back(fe);
		ans.push_back(se);
	}
	vector<Edge*> out;
	for(Edge* e : ans)
		if(e->alive){e->site_id = what_on[e->site_id]; out.push_back(e);}
		else delete e;
	return out;
}
