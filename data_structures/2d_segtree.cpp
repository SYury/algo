#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define Df(i, l, r) for(auto i = l; i != r; i--)
#define I(i, a) for(auto i : a)
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
#define fi0(x) memset(x, 0, sizeof(x))
#define finf(x) memset(x, 127, sizeof(x))
#define acpy(y, x) memcpy(y, x, sizeof(y))
#define y1 adjf
#define tm afhdhn

const int MAXN = 5e2 + 2;
const int TSZ = 4 * MAXN;
const lint inf = 1e18;

lint t[2][TSZ][TSZ];
int a[MAXN][MAXN];
int n, q;

void build_hor(int vv, int hv, int l, int r, int ci){
	if(l == r){
		t[0][vv][hv] = t[1][vv][hv] = a[ci][l];
		ret;
	}
	int mid = (l + r) >> 1;
	build_hor(vv, 2 * hv, l, mid, ci);
	build_hor(vv, 2 * hv + 1, mid + 1, r, ci);
	t[0][vv][hv] = min(t[0][vv][2*hv], t[0][vv][2*hv + 1]);
	t[1][vv][hv] = max(t[1][vv][2*hv], t[1][vv][2*hv + 1]);
}

void unite(int vv, int hv, int l, int r){
	t[0][vv][hv] = min(t[0][2*vv][hv], t[0][2*vv + 1][hv]);
	t[1][vv][hv] = max(t[1][2*vv][hv], t[1][2*vv + 1][hv]);
	if(l == r)ret;
	int mid = (l + r) >> 1;
	unite(vv, 2*hv, l, mid);
	unite(vv, 2*hv + 1, mid + 1, r);
}

void build(int v, int l, int r){
	if(l == r){
		build_hor(v, 1, 0, n - 1, l);
		ret;
	}
	int mid = (l + r) >> 1;
	build(2*v, l, mid);
	build(2*v+1, mid + 1, r);
	unite(v, 1, 0, n - 1);
}

pair<lint, lint> get_hor(int vv, int hv, int l, int r, int ul, int ur){
	if(ul > ur)ret mk(inf, -inf);
	if(l == ul && r == ur)ret mk(t[0][vv][hv], t[1][vv][hv]);
	int mid = (l + r) >> 1;
	pair<lint, lint> al = get_hor(vv, 2*hv, l, mid, ul, min(ur, mid));
	pair<lint, lint> ar = get_hor(vv, 2*hv + 1, mid + 1, r, max(ul, mid + 1), ur);
	ret mk(min(al.X, ar.X), max(al.Y, ar.Y));
}

pair<lint, lint> get(int v, int l, int r, int ul, int ur, int hl, int hr){
	if(ul > ur)ret mk(inf, -inf);
	if(l == ul && r == ur){
		ret get_hor(v, 1, 0, n - 1, hl, hr);
	}
	int mid = (l + r)>>1;
	pair<lint, lint> al, ar;
	al = get(2*v, l, mid, ul, min(ur, mid), hl, hr);
	ar = get(2*v + 1, mid + 1, r, max(ul, mid + 1), ur, hl, hr);
	ret mk(min(al.X, ar.X), max(al.Y, ar.Y));
}

void upd_hor(int vv, int hv, int l, int r, int pos, int d){
	if(l == r){t[0][vv][hv] = d; t[1][vv][hv] = d; ret;}
	int mid = (l + r) >> 1;
	if(pos <= mid)upd_hor(vv, 2*hv, l, mid, pos, d);
	else upd_hor(vv, 2*hv + 1, mid + 1, r, pos, d);
	t[0][vv][hv] = min(t[0][vv][2*hv], t[0][vv][2*hv + 1]);
	t[1][vv][hv] = max(t[1][vv][2*hv], t[1][vv][2*hv + 1]);
}

void upd_point(int vv, int hv, int l, int r, int pos){
	t[0][vv][hv] = min(t[0][2*vv][hv], t[0][2*vv + 1][hv]);
	t[1][vv][hv] = max(t[1][2*vv][hv], t[1][2*vv + 1][hv]);
	if(l == r)ret;
	int mid = (l + r) >> 1;
	if(pos <= mid)upd_point(vv, 2*hv, l, mid, pos);
	else upd_point(vv, 2*hv + 1, mid + 1, r, pos);
}

void upd(int v, int l, int r, int x, int y, int d){
	if(l == r){upd_hor(v, 1, 0, n - 1, y, d); ret;}
	int mid = (l + r) >> 1;
	if(x <= mid)upd(2*v, l, mid, x, y, d);
	else upd(2*v + 1, mid + 1, r, x, y, d);
	upd_point(v, 1, 0, n - 1, y);
}

int main(){
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d", &n);
	F(i, 0, n)
		F(j, 0, n)scanf("%d", &a[i][j]);
	build(1, 0, n - 1);
	scanf("%d", &q);
	F(i, 0, q){
		char tp = getc(stdin);
		while(!isalpha(tp))tp = getc(stdin);
		if(tp == 'q'){
			int x1, x2, y1, y2;
			scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
			x1--; x2--; y1--; y2--;
			pair<lint, lint> ans = get(1, 0, n - 1, x1, x2, y1, y2);
			printf("%lld %lld\n", ans.Y, ans.X);
		}
		else{
			int x, y, d;
			scanf("%d%d%d", &x, &y, &d);
			x--; y--;
			upd(1, 0, n - 1, x, y, d);
		}
	}
	ret 0;
}

