#include<bits/stdc++.h>

using namespace std;

typedef long double dbl;

const dbl eps = 1e-9;
const dbl PI = 2 * acos(0);

dbl sqr(dbl x){
	return x * x;
}

struct pt{
	dbl x, y;
	pt(){}
	pt(dbl a, dbl b):x(a), y(b){}
	pt(const pt & a):x(a.x), y(a.y){}
	pt& operator = (const pt & a){x = a.x; y = a.y; return *this;}
	pt operator + (const pt & a)const{return pt(x + a.x, y + a.y);}
	pt operator - (const pt & a)const{return pt(x - a.x, y - a.y);}
	pt operator * (dbl a)const{return pt(x * a, y * a);}
	pt operator / (dbl a)const{assert(fabs(a) > eps); return pt(x / a, y / a);}
	pt& operator += (const pt & a){x += a.x; y += a.y; return *this;}
	pt& operator -= (const pt & a){x -= a.x; y -= a.y; return *this;}
	pt& operator *= (dbl a){x *= a; y *= a; return *this;}
	pt& operator /= (dbl a){assert(fabs(a) > eps); return *this;}
	bool isZero()const{return fabs(x) < eps && fabs(y) < eps;}
	bool operator == (const pt & a)const{return (*this - a).isZero();}
	dbl cross(const pt & a)const{return x * a.y - y * a.x;}
	dbl cross(pt a, pt b)const{
		a -= *this; b -= *this;
		return a.cross(b);
	}
	dbl dot(const pt & a)const{return x * a.x + y * a.y;}
	dbl dot(pt a, pt b)const{
		a -= *this; b -= *this;
		return a.dot(b);
	}
	dbl length()const{return sqrt(sqr(x) + sqr(y));}
	dbl sqrLength()const{return x * x + y * y;}
	void normalizeSelf(){*this /= length();}
	pt normalize()const{
		pt res(*this);
		res.normalizeSelf();
		return res;
	}
	dbl dist(const pt & a)const{return (*this - a).length();}
	dbl angle()const{return atan2(y, x);}
	void rotateSelf(dbl phi){
		dbl pcos = cos(phi), psin = sin(phi);
		dbl nx = x * pcos - y * psin, ny = y * pcos + x * psin;
		x = nx; y = ny;
	}
	void rotateSelf(dbl cosphi, dbl sinphi){
		dbl nx = x * cosphi - y * sinphi, ny = y * cosphi + x * sinphi;
		x = nx; y = ny;
	}
	pt rotate(dbl phi)const{
		pt res(*this);
		res.rotateSelf(phi);
		return res;
	}
	pt rotate(dbl cosphi, dbl sinphi)const{
		pt res(*this);
		res.rotateSelf(cosphi, sinphi);
		return res;
	}
};

bool lexComp(const pt & l, const pt & r){
	if(fabs(l.x - r.x) > eps){
		return l.x < r.x;
	}
	else return l.y < r.y;
}

dbl angle(pt l, pt mid, pt r){
	l -= mid; r -= mid;
	return atan2(l.cross(r), l.dot(r));
}

struct Line{
	pt p[2];
	dbl a, b, c;
	Line(){}
	void recalcEquation(){
		a = p[1].y - p[0].y;
		b = p[0].x - p[1].x;
		c = -a * p[0].x - b * p[0].y;
	}
	void normalizeEquation(){
		dbl norm = sqrt(sqr(a) + sqr(b));
		a /= norm; b /= norm; c /= norm;
		if(a < -eps || (fabs(a) < eps && b < -eps)){
			a = -a; b = -b; c = -c;
		}
	}
	Line(pt l, pt r){p[0] = l; p[1] = r; recalcEquation();}
	Line(dbl pa, dbl pb, dbl pc){
		a = pa; b = pb; c = pc;
		if(fabs(b) < eps)p[0] = pt{-c/a, 0};
		else p[0] = pt{0, -c/b};
		p[1] = pt(p[0].x - b, p[0].y + a);
	}
	pt& operator [](const int & i){return p[i];}
	Line(const Line & l){
		p[0] = l.p[0]; p[1] = l.p[1];
		a = l.a; b = l.b; c = l.c;	
	}
	vector<dbl> getEquation()const{return vector<dbl>{a, b, c};}
	vector<dbl> getNormEquation()const{
		Line tmp(*this);
		tmp.normalizeEquation();
		return tmp.getEquation();
	}
	pt getOrth()const{
		return pt(a, b);
	}
	pt getNormOrth()const{
		Line tmp(*this);
		tmp.normalizeEquation();
		return tmp.getOrth();
	}
	int signPoint(const pt & t)const{
		dbl val = a * t.x + b * t.y + c;
		if(val < -eps)return -1;
		if(val > eps)return 1;
		return 0;
	}
	bool hasPointLine(const pt & t)const{
		return signPoint(t) == 0;
	}
	bool hasPointSeg(const pt & t)const{
		return hasPointLine(t) && p[0].dot(p[1], t) > -eps && p[1].dot(p[0], t) > -eps;
	}
	dbl distToPt(const pt & t)const{
		return fabs(a * t.x + b * t.y + c)/getOrth().length();
	}
};

struct Circle{
	pt c;
	dbl r;
	Circle(){}
	Circle(dbl x, dbl y, dbl rr):c(x, y), r(rr){}
	Circle(const pt & p, dbl rr):c(p), r(rr){}
	Circle(const Circle & x):c(x.c), r(x.r){}
	Circle& operator = (const Circle & x){
		c = x.c; r = x.r;
		return *this;
	}
	dbl area()const{return PI * sqr(r);}
	dbl diam()const{return 2 * r;}
	dbl perim()const{return diam() * PI;}
	bool operator == (const Circle & a)const{
		return c == a.c && fabs(r - a.r) < eps;
	}
	pt getByAngle(dbl ang)const{
		return c + pt(r * cos(ang), r * sin(ang));
	}
};

pt projPtLine(pt p, Line l){
	pt vec = l[1] - l[0];
	return l[0] + vec * (vec.dot(p - l[0])/vec.dot(vec));
}

vector<pt> interLineLine(Line l1, Line l2){
	if(fabs(l1.getOrth().cross(l2.getOrth())) < eps){
		if(l1.hasPointLine(l2[0]))return {l1[0], l1[1]};
		else return {};
	}
	pt vec = l1.p[1] - l1.p[0];
	pt norm = l2.getOrth();
	dbl s = (-l2.c - norm.dot(l1.p[0]))/norm.dot(vec);
	return {pt(l1.p[0] + vec * s)};
}

vector<pt> interSegSeg(Line l1, Line l2){
	if(l1[0] == l1[1]){
		if(l2[0] == l2[1]){
			if(l1[0] == l2[0])return {l1[0]};
			else return {};
		}
		else{
			if(l2.hasPointSeg(l1[0]))return {l1[0]};
			else return {};
		}
	}
	if(l2[0] == l2[1]){
		if(l1.hasPointSeg(l2[0]))return {l2[0]};
		else return {};
	}
	auto li = interLineLine(l1, l2);
	if(li.empty())return li;
	if(li.size() == 2){
		if(!lexComp(l1[0], l1[1]))swap(l1[0], l1[1]);
		if(!lexComp(l2[0], l2[1]))swap(l2[0], l2[1]);
		vector<pt> res(2);
		if(lexComp(l1[0], l2[0]))res[0] = l2[0]; else res[0] = l1[0];
		if(lexComp(l1[1], l2[1]))res[1] = l1[1]; else res[1] = l2[1];
		if(res[0] == res[1])res.pop_back();
		if((int)res.size() == 2 && lexComp(res[1], res[0]))return {};
		else return res;
	}
	pt cand = li[0];
	if(l1.hasPointSeg(cand) && l2.hasPointSeg(cand))return {cand};
	else return {};
}

vector<pt> interLineCircle(Line l, Circle c){
	dbl d = l.distToPt(c.c);
	if(d > c.r + eps)return {};
	if(fabs(d - c.r) < eps){
		return {projPtLine(c.c, l)};
	}
	pt p = projPtLine(c.c, l);
	dbl lol = sqrt(sqr(c.r) - sqr(d));
	lol /= (l[1] - l[0]).length();
	return {p + (l[1] - l[0])*lol, p - (l[1] - l[0])*lol};
}

vector<pt> interCircleCircle(Circle c1, Circle c2){
	if(c1.r + eps < c2.r)swap(c1, c2);
	if(c1 == c2){
		return {c1.getByAngle(0), c1.getByAngle(PI/2), c1.getByAngle(PI)};
	}
	pt vec = c2.c - c1.c;
	dbl d = vec.length();
	dbl ang = vec.angle();
	dbl longest = max(max(c1.r, c2.r), d);
	dbl per = c1.r + c2.r + d;
	if(2 * longest > per + eps)return {};
	if(abs(2 * longest - per) < 2 * eps)return {c1.getByAngle(ang)};
	dbl cang = acos((sqr(c1.r) + sqr(d) - sqr(c2.r))/(2*c1.r*d));
	return {c1.getByAngle(ang + cang), c1.getByAngle(ang - cang)};
}

vector<pt> tangentsPtCircle(pt p, Circle c){
	dbl d = (c.c - p).length();
	if(d < c.r - eps)return {};
	if(fabs(d - c.r) < eps)return {p};
	dbl ang = acos(d/c.r);
	pt l = c.c + (p - c.c).rotate(ang) * (c.r/d), r = c.c + (p - c.c).rotate(-ang) * (c.r/d);
	return {l, r};
}

vector<Line> outerTangents(Circle c1, Circle c2){
	if(c1 == c2){return {Line(0, 0, 0)};}
	if(c1.r > c2.r)swap(c1, c2);
	dbl d = (c1.c - c2.c).length();
	if(c1.r + d < c2.r - eps)return {};
	if(fabs(c1.r - c2.r) < eps){
		dbl ang = (c2.c - c1.c).angle();
		pt l = c1.getByAngle(ang + PI/2), r = c1.getByAngle(ang - PI/2);	
		return {{l, l + (c2.c - c1.c)}, {r, r + (c2.c - c1.c)}};
	}
	pt p = c2.c + (c2.c - c1.c) * (c2.r/(c2.r - c1.r));
	if(c1.r + d < c2.r + eps){
		return {{p, p + (c2.c - c1.c).rotate(PI/2)}};
	}
	dbl ang = atan((c2.r - c1.r)/d);
	return {{p, p + (c1.c - p).rotate(ang)}, {p, p + (c1.c - p).rotate(-ang)}};
}

vector<Line> innerTangents(Circle c1, Circle c2){
	if(c1 == c2){return {};}
	if(c1.r > c2.r)swap(c1, c2);
	dbl d = (c1.c - c2.c).length();
	if(d < c1.r + c2.r - eps)return {};
	pt p = c1.c + (c2.c - c1.c) * (c1.r/(c1.r + c2.r));
	if(d < c1.r + c2.r + eps){
		return {{p, p + (c1.c - p).rotate(PI/2)}};
	}
	dbl ang = acos(c1.r/(p - c1.c).length());
	dbl cang = (p - c1.c).angle();
	pt l = c1.getByAngle(cang + ang), r = c1.getByAngle(cang - ang);
	return {{p, l}, {p, r}};
}

vector<Line> allTangents(Circle c1, Circle c2){
	auto kek = outerTangents(c1, c2), bishkek = innerTangents(c1, c2);
	for(auto lol : kek)bishkek.push_back(lol);
	return bishkek;
}

struct Polygon{
	vector<pt> p;
	Polygon():p(vector<pt>()){}
	Polygon(vector<pt> pts):p(pts){}
	int nxt(int i){return (i + 1 == (int)p.size()) ? 0 : (i + 1);}
	int prv(int i){return (i == 0) ? ((int)p.size()-1) : (i-1);}
	pt& operator[](const int & i){return p[i];}
	dbl area(){
		dbl res = 0;
		for(int i = 0; i < (int)p.size(); i++)res += p[i].cross(p[nxt(i)]);
		return res/2;
	}
	void orientCCW(){
		if(area() < 0)reverse(p.begin(), p.end());
	}
	bool insidePt(pt a){
		for(int i = 0; i < (int)p.size(); i++){
			if(Line(p[i], p[nxt(i)]).hasPointSeg(a))return true;
		}
		int wn = 0;
		for(int i = 0; i < (int)p.size(); i++){
			int j = nxt(i);
			if(p[i].y < a.y + eps){
				if(a.y + eps < p[j].y){
					if(p[i].cross(p[j], a) > eps)++wn;
				}
			}
			else{
				if(p[j].y < a.y + eps){
					if(p[i].cross(p[j], a) < -eps)--wn;
				}
			}
		}
		return wn != 0;
	}
};
