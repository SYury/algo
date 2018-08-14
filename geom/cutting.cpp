// ADD BASIC GEOM HERE

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
