//ADD BASIC GEOM HERE

namespace Voronoi{
	const dbl inf = 1e20;
	const pt infpt = pt(inf, inf);
	pt center(pt a, pt b, pt c){
		if(a == b){return infpt;}
		Line l(a, b);
		if(l.hasPointLine(c)){
			return infpt;
		}
		pt mid1 = (a + b)/2, mid2 = (b + c)/2;
		pt v1 = Line(a, b).getOrth(), v2 = Line(b, c).getOrth();
		auto res = interLineLine(Line(mid1, mid1 + v1), Line(mid2, mid2 + v2));
		return res[0];
	}
	class BeachSeg;
	class Event{
	public:
		dbl x;
		int id;
		mutable set<BeachSeg>::iterator it;
		Event(dbl _x):x(_x), id(-1){}
		Event(dbl _x, int _id):x(_x), id(_id){}
		bool operator < (const Event & e)const{return x < e.x;}
	};
	class BeachSeg{
	public:
		vector<pt>::iterator bg;
		int l = -1, mid = -1, r = -1;
		mutable set<Event>::iterator it;
		dbl left_y(dbl x) const {
			if(x <= (bg + mid)->x + eps)return (bg + mid)->y;
			if(l == -1)return -inf;
			if(x <= (bg + l)->x + eps)return (bg + l)->y;
			dbl a = 0.5/((bg + l)->x - x) - 0.5 / ((bg + mid)->x - x);
			dbl b = (bg + l)->y / (x - (bg + l)->x) - (bg + mid)->y / (x - (bg + mid)->x);
			dbl c = 0.5 * (sqr((bg + l)->x) + sqr((bg + l)->y) - x * x)/((bg + l)->x - x) - 0.5 * (sqr((bg + mid)->x) + sqr((bg + mid)->y) - x * x)/((bg + mid)->x - x);
			return (-b - sqrt(max((dbl)0, b * b - 4 * a * c)))/(2*a);
		}
		dbl right_y(dbl x) const {
			if(x <= (bg + mid)->x + eps)return (bg + mid)->y;
			if(r == -1)return inf;
			if(x <= (bg + r)->x + eps)return (bg + r)->y;
			dbl a = 0.5/((bg + r)->x - x) - 0.5 / ((bg + mid)->x - x);
			dbl b = (bg + r)->y / (x - (bg + r)->x) - (bg + mid)->y / (x - (bg + mid)->x);
			dbl c = 0.5 * (sqr((bg + r)->x) + sqr((bg + r)->y) - x * x)/((bg + r)->x - x) - 0.5 * (sqr((bg + mid)->x) + sqr((bg + mid)->y) - x * x)/((bg + mid)->x - x);
			return (-b + sqrt(max((dbl)0, b * b - 4 * a * c)))/(2*a);
		}
		dbl get_x() const {
			if(l == -1 || r == -1)return inf;
			if((bg + mid)->cross(*(bg + l), *(bg + r)) < -eps)return inf;
			pt c = center(*(bg + l), *(bg + mid), *(bg + r));
			if(c.x == inf)return inf;
			return c.x + c.dist(*(bg + mid));
		}
		bool operator < (const BeachSeg & s) const {
			dbl x = max((bg + mid)->x, (bg + s.mid)->x);
			if(l != -1)x = max(x, (bg + l)->x);
			if(r != -1)x = max(x, (bg + r)->x);
			if(s.l != -1)x = max(x, (bg + s.l)->x);
			if(s.r != -1)x = max(x, (bg + s.r)->x);
			return right_y(x) <= s.left_y(x) + eps;
		}
	};
	// It is a good idea to rotate points by some random angle beforehand.
	// It is also a good idea to add some boundary points like (+-inf, +-inf).
	map<pair<int, int>, pt> voronoi(vector<pt> & pts){
		sort(pts.begin(), pts.end(), [](const pt & l, const pt & r){return l.x < r.x - eps || (l.x < r.x + eps && l.y < r.y - eps);});
		map<pair<int, int>, pt> ans;
		auto insert_edge = [&ans](int l, int r, pt vertex){
			if(ans.count(mp(l, r)) && ans[mp(l, r)] != vertex)
				ans[mp(r, l)] = vertex;
			else ans[mp(l, r)] = vertex;
		};
		multiset<Event> events;
		multiset<BeachSeg> beach_line;
		for(int i = 0; i < (int)pts.size(); i++)
			events.insert(Event(pts[i].x, i));
		while(!events.empty()){
			Event curr = *events.begin();
			if(curr.x >= inf)break;
			events.erase(events.begin());
			if(curr.id >= 0){
				BeachSeg tmp;
				tmp.bg = pts.begin();
				tmp.mid = curr.id;
				auto it = beach_line.lower_bound(tmp);
				if(it != beach_line.end()){
					tmp.l = tmp.r = it->mid;
					BeachSeg to_add = *it;
					int l = to_add.l;
					if(it->it != events.end())events.erase(it->it);
					beach_line.erase(it);
					to_add.l = curr.id;
					to_add.it = events.insert(Event(to_add.get_x(), -1));
					to_add.it->it = beach_line.insert(to_add);
					to_add.l = l;
					to_add.r = curr.id;
					to_add.it = events.insert(Event(to_add.get_x(), -1));
					to_add.it->it = beach_line.insert(to_add);
				}
				tmp.it = events.insert(Event(tmp.get_x(), -1));
				tmp.it->it = beach_line.insert(tmp);
			}
			else{
				BeachSeg s = *curr.it;
				pt vertex = center(pts[s.l], pts[s.mid], pts[s.r]);
				insert_edge(s.l, s.r, vertex);
				insert_edge(s.mid, s.l, vertex);
				insert_edge(s.r, s.mid, vertex);
				auto il = curr.it, ir = curr.it;
				--il; ++ir;
				BeachSeg ls = *il, rs = *ir;
				beach_line.erase(curr.it);
				events.erase(il->it);
				beach_line.erase(il);
				events.erase(ir->it);
				beach_line.erase(ir);
				ls.r = rs.mid;
				rs.l = ls.mid;
				ls.it = events.insert(Event(ls.get_x(), -1));
				ls.it->it = beach_line.insert(ls);
				rs.it = events.insert(Event(rs.get_x(), -1));
				rs.it->it = beach_line.insert(rs);
			}
		}
		pair<int, int> la = {-1, -1};
		for(BeachSeg s : beach_line){
			if(s.l != -1)
				insert_edge(s.l, s.mid, infpt);
			if(s.r != -1)
				la = {s.mid, s.r};
		}
		if(la.first != -1)
			insert_edge(la.first, la.second, infpt);
		return ans;
	}
};
