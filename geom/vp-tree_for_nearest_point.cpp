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
#define I(a, x) for(auto a : x)
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
#define acpy(y, x) memcpy(y, x, sizeof(y))
#define y1 adjf
#define tm fjfjfk

const lint inf = 2e18 + 228;
const int SZ = 1000 * 1000 * 500;
int mem[SZ];
size_t mem_ptr = 0;

inline void* operator new(size_t len){
	mem_ptr += len;
	ret mem + mem_ptr - len;
}

inline void operator delete(void*){}

const int MAXN = 1e5 + 5;

typedef pair<lint, lint> pt;

struct node{
	lint rad;
	pt v;
	node *l, *r;
};

lint dist(pt a, pt b){
	ret (a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y);
}

struct comp{
	pt v;
	comp(){}
	comp(pt _v):v(_v){}
	bool operator () (const pt & l, const pt & r){
		ret dist(v, l) < dist(v, r);
	}
};

pt p[MAXN];
int n;

node* build(int l, int r){
	if(l > r)ret NULL;
	node* v = new node;
	if(l == r){
		v->rad = 0;
		v->v = p[l];
		v->l = v->r = NULL;
		ret v;
	}
	int pos = l + rand()%(r - l + 1);
	swap(p[l], p[pos]);
	v->v = p[l];
	sort(p+l+1, p+r+1, comp(p[l]));
	v->rad = dist(p[l], p[(l+r)>>1]);
	v->l = build(l, (l+r)>>1);
	v->r = build(((l+r)>>1) + 1, r);
	ret v;
}

void get(node* v, pt q, lint & ans){
	if(!v)ret;
	lint d = dist(v->v, q);
	if(v->v != q)ans = min(ans, dist(v->v, q));
	if(v->l == NULL && v->r == NULL)ret;
	if(d <= v->rad){
		get(v->l, q, ans);
		if(sqrt(d) + sqrt(ans) > sqrt(v->rad))get(v->r, q, ans);
	}
	else{
		get(v->r, q, ans);
		if(sqrt(d) - sqrt(ans) < sqrt(v->rad))get(v->l, q, ans);
	}
}

int t;
node * root;
lint ans;
pt kekus_maximus[MAXN];

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d", &t);
	F(g, 0, t){
		mem_ptr = 0;
		scanf("%d", &n);
		F(i, 0, n)scanf("%lld%lld", &p[i].X, &p[i].Y);
		F(i, 0, n)kekus_maximus[i] = p[i];
		root = build(0, n - 1);
		F(i, 0, n){
			ans = inf;
			get(root, kekus_maximus[i], ans);
			printf("%lld\n", ans);
		}
	}
	ret 0;
}
