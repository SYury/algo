// ADD BASIC GEOM HERE

struct circ_edge{
	pt l, r;
	bool is_arc;
};

// It doesn't add tangency points if they aren't needed to build the intersection
vector<circ_edge> convex_circle_inter(Polygon p, Circle c){
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


Polygon convex_intersection(Polygon a, Polygon b){ //intersection of convex polygons in O(n)
	vector<dbl> xpos(a.size() + b.size());
	int xptr = 0;
	vector<dbl> tmp(a.size() + b.size());
	if(a.area() > 0)reverse(a.p.begin(), a.p.end());
	if(b.area() > 0)reverse(b.p.begin(), b.p.end());
	int i, j, k, oi, ok;
	for(int useless_var = 0; useless_var < 2; useless_var++){
		i = 0; k = 0;
		for(j = 1; j < a.size(); j++){
			if(lt(a[j].x, a[i].x) || (eq(a[j].x, a[i].x) && lt(a[j].y, a[i].y)))i = j;
			if(gt(a[j].x, a[k].x) || (eq(a[j].x, a[k].x) && gt(a[j].y, a[i].y)))k = j;
		}
		j = i;
		vector<dbl> lol;
		while(j != k){
			if(lol.empty() || !eq(lol.back(), a[j].x))lol.push_back(a[j].x);
			j = a.nxt(j);
		}
		merge(lol.begin(), lol.end(), xpos.begin(), xpos.begin() + xptr, tmp.begin(), lt);
		xptr += lol.size();
		for(int idx = 0; idx < xptr; idx++)xpos[idx] = tmp[idx];
		lol.clear();
		while(j != i){
			if(lol.empty() || !eq(lol.back(), a[j].x))lol.push_back(a[j].x);
			j = a.nxt(j);
		}
		reverse(lol.begin(), lol.end());
		merge(lol.begin(), lol.end(), xpos.begin(), xpos.begin() + xptr, tmp.begin(), lt);
		xptr += lol.size();
		for(int idx = 0; idx < xptr; idx++)xpos[idx] = tmp[idx];
		lol.clear();
		swap(i, oi);
		swap(k, ok);
		a.p.swap(b.p);
	}
	xpos.resize(unique(xpos.begin(), xpos.end(), eq) - xpos.begin());
	vector<dbl>().swap(tmp);
	vector<dbl> x(xpos.begin(), xpos.end());
	vector<dbl>().swap(xpos);
	int SX = x.size();
	vector<vector<Line> > vert(SX);
	vector<vector<Line> > up(SX), down(SX);
	for(int useless_var = 0; useless_var < 2; useless_var++){
		j = i;
		int pos = 0;
		while(lt(x[pos], a[i].x))++pos;
		while(j != k){
			int j0 = a.nxt(j);
			pt left_p = a[j];
			Line curr_line = Line(a[j], a[j0]);
			if(eq(a[j].x, a[j0].x)){vert[pos].push_back(a[j].y < a[j0].y ? Line(a[j], a[j0]) : Line(a[j0], a[j])); j = j0; continue;}
			while(lt(x[pos], a[j0].x)){
				pt right_p = interLineLine(curr_line, Line(pt(x[pos + 1], 0), pt(x[pos + 1], 1)))[0];
				up[pos].push_back(Line(left_p, right_p));
				++pos;
				left_p = right_p;
			}
			j = j0;
		}
		while(j != i){
			int j0 = a.nxt(j);
			pt right_p = a[j];
			Line curr_line = Line(a[j], a[j0]);
			if(eq(a[j].x, a[j0].x)){vert[pos].push_back(a[j].y < a[j0].y ? Line(a[j], a[j0]) : Line(a[j0], a[j])); j = j0; continue;}
			while(gt(x[pos], a[j0].x)){
				pt left_p = interLineLine(curr_line, Line(pt(x[pos - 1], 0), pt(x[pos - 1], 1)))[0];
				down[pos - 1].push_back(Line(left_p, right_p));
				--pos;
				right_p = left_p;
			}
			j = j0;
		}
		swap(i, oi);
		swap(k, ok);
		a.p.swap(b.p);
	}
	vector<pt> up_hull, down_hull;
	auto add_point = [](vector<pt> & v, const pt & p){
		while(!v.empty() && v.back() == p)return;
		while(v.size() >= 2u && eq(fabs(v[v.size() - 1].cross(v[v.size() - 2], p)), 0))v.pop_back();
		v.push_back(p);
	};
	for(int pos = 0; pos < SX; pos++){
		if(vert[pos].size() == 2u){
			dbl r = min(vert[pos][0][1].y, vert[pos][1][1].y);
			dbl l = max(vert[pos][0][0].y, vert[pos][1][0].y);
			if(lt(l, r)){add_point(up_hull, pt(x[pos], r)); add_point(down_hull, pt(x[pos], l));}
			else if(eq(l, r))add_point(up_hull, pt(x[pos], (l + r)/2));
		}
		if(up[pos].size() < 2u)continue;
		assert(down[pos].size() == 2u);
		auto kek0 = interSegSeg(up[pos][0], down[pos][1]);
		auto kek1 = interSegSeg(up[pos][1], down[pos][0]);
		auto kek2 = interSegSeg(up[pos][0], up[pos][1]);
		auto kek3 = interSegSeg(down[pos][0], down[pos][1]);
		if(kek0.size() == 2u){
			add_point(up_hull, up[pos][0][0]);
			add_point(up_hull, up[pos][0][1]);
			continue;
		}
		if(kek1.size() == 2u){
			add_point(up_hull, up[pos][1][0]);
			add_point(up_hull, up[pos][1][1]);
			continue;
		}
		size_t tot = kek1.size() + kek0.size();
		int base_d;
		pt l, r;
		switch(tot){
			case 0:
				l = up[pos][0][0].y < up[pos][1][0].y ? up[pos][0][0] : up[pos][1][0];
				r = down[pos][0][0].y > down[pos][1][0].y ? down[pos][0][0] : down[pos][1][0];
				if(r.y > l.y)break;
				add_point(up_hull, up[pos][0][0].y < up[pos][1][0].y ? up[pos][0][0] : up[pos][1][0]);
				if(kek2.size() == 1u)add_point(up_hull, kek2[0]);
				add_point(up_hull, up[pos][0][1].y < up[pos][1][1].y ? up[pos][0][1] : up[pos][1][1]);
				add_point(down_hull, down[pos][0][0].y > down[pos][1][0].y ? down[pos][0][0] : down[pos][1][0]);
				if(kek3.size() == 1u)add_point(down_hull, kek3[0]);
				add_point(down_hull, down[pos][0][1].y > down[pos][1][1].y ? down[pos][0][1] : down[pos][1][1]);
			break;
			case 1:
				base_d = kek1.empty();
				if(down[pos][base_d][1].y < up[pos][base_d^1][1].y){
					add_point(up_hull, kek1.empty() ? kek0[0] : kek1[0]);
					if(kek2.size() == 1u)add_point(up_hull, kek2[0]);
					add_point(up_hull, up[pos][0][1].y < up[pos][1][1].y ? up[pos][0][1] : up[pos][1][1]);
					if(kek3.size() == 1u)add_point(down_hull, kek3[0]);
					add_point(down_hull, down[pos][0][1].y > down[pos][1][1].y ? down[pos][0][1] : down[pos][1][1]);
				}
				else{	
					add_point(up_hull, up[pos][0][0].y < up[pos][1][0].y ? up[pos][0][0] : up[pos][1][0]);
					if(kek2.size() == 1u)add_point(up_hull, kek2[0]);
					add_point(up_hull, kek1.empty() ? kek0[0] : kek1[0]);
					add_point(down_hull, down[pos][0][0].y > down[pos][1][0].y ? down[pos][0][0] : down[pos][1][0]);
					if(kek3.size() == 1u)add_point(down_hull, kek3[0]);
				}
			break;
			case 2:
				l = kek1[0];
				r = kek0[0];
				if(l.x > r.x)swap(l, r);
				add_point(up_hull, l);
				if(kek2.size() == 1u)add_point(up_hull, kek2[0]);
				add_point(up_hull, r);
				if(kek3.size() == 1u)add_point(down_hull, kek3[0]);
			break;
		};
	}
	for(int pos = (int)down_hull.size() - 1; pos >= 0; pos--){
		if(up_hull.empty() || up_hull.back() != down_hull[pos])up_hull.push_back(down_hull[pos]);
	}
	return up_hull;
}
