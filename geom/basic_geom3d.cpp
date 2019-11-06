#include<bits/stdc++.h>

using namespace std;

typedef long double dbl;

constexpr dbl eps = 1e-9;
constexpr dbl PI = 2 * acos(0);

constexpr inline dbl safe_sqrt(dbl x){
    return x < 0 ? 0 : sqrt(x);
}

constexpr inline dbl safe_acos(dbl x){
    return x < -1 ? acos(-1) : (x > 1 ? acos(1) : acos(x));
}

constexpr inline dbl safe_asin(dbl x){
    return x < -1 ? asin(-1) : (x > 1 ? asin(1) : asin(x));
}

constexpr inline dbl sqr(dbl x){
    return x * x;
}

constexpr inline bool eq(dbl x, dbl y){
    return fabs(x - y) < eps;
}

constexpr inline bool gt(dbl x, dbl y){
    return x > y + eps;
}

constexpr inline bool lt(dbl x, dbl y){
    return y > x + eps;
}

constexpr inline bool ge(dbl x, dbl y){
    return !lt(x, y);
}

constexpr inline bool le(dbl x, dbl y){
    return !gt(x, y);
}

struct pt{
    dbl x, y, z;
    pt():x(0), y(0), z(0){}
    pt(dbl _x, dbl _y, dbl _z):x(_x), y(_y), z(_z){}
    pt operator + (const pt & a)const{return pt(x + a.x, y + a.y, z + a.z);}
    pt operator - (const pt & a)const{return pt(x - a.x, y - a.y, z - a.z);}
    pt operator * (dbl a)const{return pt(x * a, y * a, z * a);}
    pt operator / (dbl a)const{assert(abs(a) > eps); return pt(x / a, y / a, z / a);}
    pt& operator += (const pt & a){x += a.x; y += a.y; z += a.z; return *this;}
    pt& operator -= (const pt & a){x -= a.x; y -= a.y; z -= a.z; return *this;}
    pt& operator *= (dbl a){x *= a; y *= a; z *= a; return *this;}
    pt& operator /= (dbl a){assert(abs(a) > eps); x /= a; y /= a; z /= a; return *this;}
    bool operator == (const pt & p)const{return eq(x, p.x) && eq(y, p.y) && eq(z, p.z);}
    bool operator != (const pt & p)const{return !(*this == p);}
    dbl dot(const pt & a)const{return x * a.x + y * a.y + z * a.z;}
    dbl dot(const pt & a, const pt & b)const{return (a - *this).dot(b - *this);}
    pt cross(const pt & a)const{return pt(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);}
    pt cross(const pt & a, const pt & b)const{return (a - *this).cross(b - *this);}
    dbl mixed(const pt & a, const pt & b, const pt & c)const{return (a - *this).dot((b - *this).cross(c - *this));}
    dbl sqrLength()const{return this->dot(*this);}
    dbl length()const{return sqrt(sqrLength());}
    dbl dist(const pt & a)const{return (a - *this).length();}
    pt normalize()const{
        return *this/this->length();
    }
    void out()const{cout << x << " " << y << " " << z << '\n';}
};

dbl mixed(const pt & a, const pt & b, const pt & c){return pt(0, 0, 0).mixed(a, b, c);}

struct Line{
    pt p[2];
    Line(){}
    Line(pt a, pt b){p[0] = a; p[1] = b;}
    pt& operator [](const int & i){return p[i];}
};

struct Plane{
    pt p[3];
    Plane(){}
    Plane(pt a, pt b, pt c){p[0] = a; p[1] = b; p[2] = c;}
    pt& operator [](const int & i){return p[i];}
    pt getOrth()const{return p[0].cross(p[1], p[2]);}
    int signPoint(const pt & a)const{
        dbl val = p[0].mixed(a, p[1], p[2]);
        if(val < -eps)return -1;
        if(val > eps)return 1;
        return 0;
    }
    bool hasPointPlane(const pt & a)const{
        return signPoint(a) == 0;
    }
    dbl distPointPlane(const pt & a)const{
        return abs(p[0].mixed(a, p[1], p[2])/p[0].cross(p[1], p[2]).length());
    }
};

struct Sphere{
    pt c;
    dbl r;
    Sphere(){}
    Sphere(pt cc, dbl rr):c(cc), r(rr){}
    bool inPtSphere(const pt & p)const{return lt(c.dist(p), r);}
    bool onPtSphere(const pt & p)const{return eq(c.dist(p), r);}
    bool hasPtSphere(const pt & p)const{return le(c.dist(p), r);}
    dbl distBetween(const pt & a, const pt & b)const{return r * safe_acos(a.dot(b)/(r * r));}
};

pt projPtLine(pt p, Line l){
    return l[0] + (l[1] - l[0]) * (l[0].dot(p, l[1])/l[0].dot(l[1], l[1]));
}

pt projPtPlane(pt p, Plane pl){
    return pl[0] + (pl[1] - pl[0]) * (pl[0].dot(p, pl[1])/pl[0].dot(pl[1], pl[1]))
        + (pl[2] - pl[0]) * (pl[0].dot(p, pl[2])/pl[0].dot(pl[2], pl[2]));
}

pair<dbl, dbl> getPlaneCoordinates(pt p, Plane pl){
    pt xvec = (pl[1] - pl[0]).normalize();
    pt norm = pl.getOrth();
    pt yvec = xvec.cross(norm).normalize();
    return {(p - pl[0]).dot(xvec), (p - pl[0]).dot(yvec)};
}

vector<pt> interLineSphere(Line l, Sphere s){
    pt kek = projPtLine(s.c, l);
    dbl d = kek.dist(s.c);
    if(gt(d, s.r))
        return {};
    if(eq(d, s.r))
        return {kek};
    dbl lol = safe_sqrt(s.r * s.r - d * d);
    lol /= (l[1] - l[0]).length();
    return {kek - (l[1] - l[0]) * lol, kek + (l[1] - l[0]) * lol};
}
