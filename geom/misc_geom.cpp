// ADD BASIC GEOM HERE

// BEGIN CUT CONVEX POLYGON

vector<pt> cutConvex(Polygon p, Line ln, Polygon & l, Polygon & r){
	int n = p.size();
	l.clear(); r.clear();
	bool side = false;
	vector<pt> cutp;
	for(int i = 0; i < n; i++){
		int j = p.nxt(i);
		auto cand = interLineSeg(ln, {p[i], p[j]});
		if(cand.empty()){
			if(!side){l.push_back(p[j]);}
			else {r.push_back(p[j]);}
			continue;
		}
		if(cand.size() == 2){
			l = Polygon();
			r = p;
			return cand;
		}
		pt curr = cand[0];
		if(curr == p[i]){
			if(!side){l.push_back(p[i]); l.push_back(p[j]); }else {r.push_back(p[i]); r.push_back(p[j]);}
			continue;
		}
		if(curr == p[j]){
			cutp.push_back(p[j]);
			if(!side)l.push_back(p[j]); else r.push_back(p[j]);
			side = !side;
			continue;
		}
		cutp.push_back(curr);
		if(!side){l.push_back(curr); r.push_back(curr); r.push_back(p[j]);}
		else {r.push_back(curr); l.push_back(curr); l.push_back(p[j]);}
		side = !side;
	}
	if(cutp.size() == 1){
		l = Polygon();
		r = p;
	}
	return cutp;
}

// END CUT CONVEX POLYGON

// BEGIN MIN SPANNING CIRCLE

Circle minCircle(vector<pt> what){
	srand(time(0));
	int n = what.size();
	for(int i = 0; i < n; i++){
		int j = rand()%(i + 1);
		swap(what[i], what[j]);
	}
	if(what.empty())return Circle({0, 0}, 0);
	Circle ans(what[0], 0);
	for(int i = 1; i < n; i++){
		if(ans.c.dist(what[i]) < ans.r + eps)continue;
		ans = Circle((what[0] + what[i])/2, what[0].dist(what[i])/2);
		for(int j = 0; j <= i; j++){
			if(ans.c.dist(what[j]) < ans.r + eps)continue;
			ans = Circle((what[j] + what[i])/2, what[i].dist(what[j])/2);
			for(int k = 0; k <= j; k++){
				if(ans.c.dist(what[k]) < ans.r + eps)continue;
				pt cen = trCirc(what[i], what[j], what[k]);
				ans = Circle(cen, cen.dist(what[i]));
			}
		}
	}
	return ans;
}

// END MIN SPANNING CIRCLE

// BEGIN CIRCLE CONVEX POLYGON INTERSECTION

// I'm not sure it works properly if the circle's center is not inside the polygon
// It doesn't add tangency points if they aren't needed to build the intersection

struct circ_edge{
	pt l, r;
	bool is_arc;
};

vector<circ_edge> calc_inter(Polygon p, Circle c){
	vector<circ_edge> edges;
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

// END CIRCLE CONVEX POLYGON INTERSECTION

// BEGIN CUT SIMPLE POLYGON

dbl cutPolygon(Polygon & p, Line l){
	int n = p.size();
	vector<pair<dbl, int> > events;
	for(int i = 0; i < n; i++){
		int j = p.nxt(i);
		int is = l.signPoint(p[i]), js = l.signPoint(p[j]);
		if(is == js)continue;
		dbl pos = (l[1] - l[0]).dot(interLineLine(l, Line(p[i], p[j]))[0] - l[0])/(l[1] - l[0]).length();
		if(is < js)events.push_back(make_pair(pos, is && js ? 2 : 1));
		else events.push_back(make_pair(pos, is && js ? -2 : -1));
	}
	sort(events.begin(), events.end());
	int bal = 0;
	dbl ans = 0;
	F(i, 0, (int)events.size()){
		if(bal)ans += events[i].first - events[i - 1].first;
		bal += events[i].second;
	}
	return ans;
}

// END CUT SIMPLE POLYGON
