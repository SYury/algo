bool insidePtPoly(const Polygon & p, pt a){
	for(int i = 0; i < (int)p.p.size(); i++){
		if(Line(p.p[i], p.p[p.nxt(i)]).hasPointSeg(a))return true;
	}
	int wn = 0;
	for(int i = 0; i < (int)p.p.size(); i++){
		int j = p.nxt(i);
		if(p.p[i].y < a.y + eps){
			if(a.y + eps < p.p[j].y){
				if(p.p[i].cross(p.p[j], a) > eps)++wn;
			}
		}
		else{
			if(p.p[j].y < a.y + eps){
				if(p.p[i].cross(p.p[j], a) < -eps)--wn;
			}
		}
	}
	return wn != 0;
}
