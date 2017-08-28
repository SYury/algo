#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define Df(i, l, r) for(auto i = l; i != r; i--)
#define uF(i, l, r) for(i = l; i != r; i++)
#define uDf(i, l, r) for(i = l; i != r; i--)
#define pb push_back
#define rs resize
#define mk make_pair
#define asg assign
#define all(x) x.begin(),x.end()
#define ret return
#define cont continue
#define brk break
#define ins insert
#define era erase
#define y1 adjf

const int inf = 1e9 + 7;

struct node{
	node *l, *r;
	bool isx;
	int x, y;
	int minx, maxx, miny, maxy;
	int val, pu;
	int col = -1;
};


bool insidep(int x, int y, int x1, int y1, int x2, int y2){
	ret (x1 <= x) && (x <= x2) && (y1 <= y) && (y <= y2);
}


void push(node * v){
	if(!v || v->col == -1)
		ret;
	if(v->l){
		v->l->val = v->l->col = v->l->pu = v->col;
	}
	if(v->r){
		v->r->val = v->r->col = v->r->pu = v->col;
	}
	v->col = -1;
}

struct pt{
	int x, y, val, id;
};

auto xcomp = [](const pt & a, const pt & b){
	ret (a.x < b.x) || (a.x == b.x && a.y < b.y);
};

auto ycomp = [](const pt & a, const pt & b){
	ret (a.y < b.y) || (a.y == b.y && a.x < b.x);
};

vector<pt> sx, sy;
int n, m;
node * root;

int val(node * v){
	if(!v)
		ret inf;
	ret v->val;
}

node * build(bool isx, vector<pt> & sx, vector<pt> & sy){
	node * v = new node;
	v->isx = isx;
	v->minx = sx[0].x;
	v->maxx = sx.back().x;
	v->miny = sy[0].y;
	v->maxy = sy.back().y;
	//printf("B: minx = %d miny = %d maxx = %d maxy = %d\n", v->minx, v->miny, v->maxx, v->maxy);
	if(sx.size() == 1){
		v->x = sx[0].x;
		v->y = sx[0].y;
		v->val = sx[0].val;
		v->pu = sx[0].val;
		ret v;
	}
	if(isx){
		int pos = sx.size()/2;
		//while(pos > 0 && sx[pos - 1].x == sx[pos].x){pos--;}
		v->x = sx[pos].x;
		v->y = sx[pos].y;
		v->pu = sx[pos].val;
		int szl = pos, szr = sx.size() - pos - 1;
		v->val = sx[pos].val;
		vector<pt> lx, rx, ly, ry;
		if(szl != 0){
			lx.rs(szl); ly.rs(szl);
			F(i, 0, pos){lx[i] = sx[i];}
			int cp = 0;
			F(i, 0, (int)sy.size()){
				if(xcomp(sy[i], sx[pos]))
					ly[cp++] = sy[i];
			}
		}
		if(szr != 0){
			ry.rs(szr); rx.rs(szr);
			F(i, pos + 1, (int)sx.size()){
				rx[i - pos - 1] = sx[i];
			}
			int cp = 0;
			F(i, 0, (int)sy.size()){
				if(!xcomp(sy[i], sx[pos]) && sx[pos].id != sy[i].id)
					ry[cp++] = sy[i];
			}
		}
		sx.clear(); sy.clear();
		if(szl != 0)
			v->l = build(!isx, lx, ly);
		if(szr != 0)
			v->r = build(!isx, rx, ry);
		v->val = min(v->val, val(v->l));
		v->val = min(v->val, val(v->r));
	}
	if(!isx){
		int pos = sy.size()/2;
		//while(pos > 0 && sy[pos - 1].y == sy[pos].y){pos--;}
		v->x = sy[pos].x;
		v->y = sy[pos].y;
		v->pu = sy[pos].val;
		int szl = pos, szr = sy.size() - pos - 1;
		v->val = sy[pos].val;
		vector<pt> lx, rx, ly, ry;
		if(szl != 0){
			lx.rs(szl); ly.rs(szl);
			F(i, 0, pos){ly[i] = sy[i];}
			int cp = 0;
			F(i, 0, (int)sx.size()){
				if(ycomp(sx[i], sy[pos]))
					lx[cp++] = sx[i];
			}
		}
		if(szr != 0){
			ry.rs(szr); rx.rs(szr);
			F(i, pos + 1, (int)sy.size()){
				ry[i - pos - 1] = sy[i];
			}
			int cp = 0;
			F(i, 0, (int)sx.size()){
				if(!ycomp(sx[i], sy[pos]) && sy[pos].id != sx[i].id)
					rx[cp++] = sx[i];
			}
		}
		sx.clear(); sy.clear();
		if(szl != 0)
			v->l = build(!isx, lx, ly);
		if(szr != 0)
			v->r = build(!isx, rx, ry);
		v->val = min(v->val, val(v->l));
		v->val = min(v->val, val(v->r));
	}
	ret v;
}

bool insider(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
	ret (x3 <= x1) && (x1 <= x4) && (y3 <= y1) && (y1 <= y4) && (x3 <= x2) && (x2 <= x4) && (y3 <= y2) && (y2 <= y4);

}


int get(node * v, int x1, int y1, int x2, int y2){
//	if(v){printf("Q: minx = %d miny = %d maxx = %d maxy = %d x1 = %d y1 = %d x2 = %d y2 = %d and v->val = %d\n", v->minx, v->miny, v->maxx, v->maxy, x1, y1, x2, y2, v->val);}
	push(v);
	if(!v){ret inf;}
	if(v->maxx < x1 || v->minx > x2 || x1 > x2 || v->maxy < y1 || v->miny > y2 || y1 > y2){ret inf;}
	if(insider(v->minx, v->miny, v->maxx, v->maxy, x1, y1, x2, y2)){ret v->val;}
	int cv = insidep(v->x, v->y, x1, y1, x2, y2) ? v->pu : inf;
	if(v->isx){
		cv = min(cv, get(v->l, x1, y1, min(x2, v->x), y2));
		cv = min(cv, get(v->r, max(x1, v->x), y1, x2, y2));
		ret cv;
	}
	else{
		cv = min(cv, get(v->l, x1, y1, x2, min(y2, v->y)));
		cv = min(cv, get(v->r, x1, max(y1, v->y), x2, y2));
		ret cv;
	}
}

void upd(node * v, int x1, int y1, int x2, int y2, int col){
	//if(v){printf("Q: minx = %d miny = %d maxx = %d maxy = %d x1 = %d y1 = %d x2 = %d y2 = %d val = %d\n", v->minx, v->miny, v->maxx, v->maxy, x1, y1, x2, y2, v->val);}
	push(v);
	if(!v){ret;}
	if(v->maxx < x1 || v->minx > x2 || x1 > x2 || v->maxy < y1 || v->miny > y2 || y1 > y2){ret;}
	if(insider(v->minx, v->miny, v->maxx, v->maxy, x1, y1, x2, y2)){v->val = v->col = v->pu = col; ret;}
	if(insidep(v->x, v->y, x1, y1, x2, y2)){v->val = v->pu = col;}else{v->val = v->pu;}
	if(v->isx){
		upd(v->l, x1, y1, min(x2, v->x), y2, col);
		upd(v->r, max(x1, v->x), y1, x2, y2, col);
	}
	else{
		upd(v->l, x1, y1, x2, min(y2, v->y), col);
		upd(v->r, x1, max(y1, v->y), x2, y2, col);
	}
	v->val = min(v->val, val(v->l));
	v->val = min(v->val, val(v->r));
//	if(v){printf("Q-updated: minx = %d miny = %d maxx = %d maxy = %d x1 = %d y1 = %d x2 = %d y2 = %d val = %d\n", v->minx, v->miny, v->maxx, v->maxy, x1, y1, x2, y2, v->val);}
}

char getq(){
	char c = '!';
	while(c != '?' && c != '=')
		c = getc(stdin);
	ret c;
}

map<pair<int, int>, int> bproc;
vector<pt> tmr;

int main(){
//	freopen("input.txt", "r", stdin);
	scanf("%d", &n);
	sx.rs(n); sy.rs(n); tmr.rs(n);
	int pos = 0;
	F(i, 0, n){
		pt tmp;
		scanf("%d%d%d", &tmp.x, &tmp.y, &tmp.val);
		tmp.id = i;
		tmr[i] = tmp;
		pair<int, int> pp = mk(tmp.x, tmp.y);
		if(!bproc.count(pp)){bproc[pp] = tmp.val;}
		else{bproc[pp] = min(bproc[pp], tmp.val);}
	}
	F(i, 0, n){
		pair<int, int> pp = mk(tmr[i].x, tmr[i].y);
		if(bproc[pp] != -1){
			tmr[i].val = bproc[pp];
			sx[pos] = tmr[i];
			sy[pos++] = tmr[i];
			bproc[pp] = -1;
		}
	}
	n = pos;
	sx.rs(pos); sy.rs(pos);
	sort(sx.begin(), sx.end(), xcomp);
	sort(sy.begin(), sy.end(), ycomp);
	root = build(true, sx, sy);
	scanf("%d", &m);
	F(i, 0, m){
		char tp = getq();
		int x1, y1, x2, y2;
		scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
		if(tp == '?'){
			int ans =  get(root, x1, y1, x2, y2);
			if(ans == inf){printf("NO\n");}
			else{printf("%d\n", ans);}
		}
		else{
			int col;
			scanf("%d", &col);
			upd(root, x1, y1, x2, y2, col);
		}
	}
	ret 0;
}

