//ADD BASIC GEOM HERE

//I'm not sure it work properly if the circle's center is not inside the polygon
//It doesn't add tangency points if they aren't needed to build the intersection

struct edge{
	pt l, r;
	bool is_arc;
};

vector<edge> calc_inter(Polygon p, Circle c){
	vector<edge> edges;
	for(int i = 0; i < p.size(); i++)p.p[i] -= c.c;
	c.c = {0, 0};
	pt la;
	bool was_last = false;
	bool was_wp = false;
	pt where;
	pt tp;
	bool touched = false;
	for(int i = 0; i < p.size(); i++){
		int j = p.nxt(i);
		Line li(p[i], p[j]);
		auto kek = interSegCircle(li, c);
		if(kek.size() == 2){
			pt l = kek[0], r = kek[1];
			if(p[i].dist(l) > p[i].dist(r))swap(l, r);
			edges.push_back({l, r, false});
			if(was_last)edges.push_back({la, l, true});
			else{where = l; was_wp = true;}
			was_last = true; la = r;
		}
		if(kek.size() == 1){
			if(li.distToPt(c.c) > c.r - eps){touched = true; tp = kek[0]; continue;}
			if(c.c.dist(p[i]) < c.r + eps){
				was_last = true;
				la = kek[0];
				edges.push_back({p[i], kek[0], false});
			}
			else{
				if(was_last)edges.push_back({la, kek[0], true});
				else{where = kek[0]; was_wp = true;}
				edges.push_back({kek[0], p[j], false});
			}
		}
		if(kek.size() == 0 && c.c.dist(p[i]) < c.r - eps){
			edges.push_back({p[i], p[j], false});
		}
	}
	if(was_wp)edges.push_back({la, where, true});
	assert(edges.size() != 1);
	if(edges.empty() && touched){edges.push_back({tp, tp, false});}
	return edges;
}
