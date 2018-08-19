// ADD BASIC GEOM HERE

inline Polygon add(Polygon l, Polygon r){
	int il = 0, ir = 0;
	int n = l.size(), m = r.size();
	for(int i = 1; i < n; i++){
		if(lt(l[i].y, l[il].y) || (eq(l[i].y, l[il].y) && lt(l[i].x, l[il].x)))il = i;
	}
	for(int i = 1; i < m; i++){
		if(lt(r[i].y, r[ir].y) || (eq(r[i].y, r[ir].y) && lt(r[i].x, r[ir].x)))ir = i;
	}
	rotate(l.p.begin(), l.p.begin() + il, l.p.end());
	rotate(r.p.begin(), r.p.begin() + ir, r.p.end());
	l.push_back(l[0]); l.push_back(l[1]);
	r.push_back(r[0]); r.push_back(r[1]);
	int i = 0, j = 0;
	Polygon res;
	while(i < n || j < m){
		res.push_back(l[i] + r[j]);
		auto cr = (l[i + 1] - l[i]).cross(r[j + 1] - r[j]);
		if(gt(cr, 0))++i;
		else if(lt(cr, 0))++j;
		else {++i; ++j;}
	}
	return res;
}
