//Wavelet tree
//kth minimum and number of occ. on segment in O(log n)
//kth occ. in O(log^2 n) and O(log n)
//Adj. swap in O(log n)

#include<bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define DF(i, l, r) for(auto i = l; i != r; i--)
#define I(i, a) for(auto i : a)
#define pb push_back
#define rs resize
#define mp make_pair
#define asg assign
#define all(x) x.begin(),x.end()
#define ret return
#define cont continue
#define brk break
#define ins insert
#define era erase
#define fi0(x) memset(x, 0, sizeof(x))
#define finf(x) memset(x, 127, sizeof(x))
#define y1 adjf
#define tm dhgdg

const int inf = 1e6 + 6;
const int MAXN = 1e6 + 6;

int a[MAXN];
int lel[MAXN];

struct node{
	int lo, hi;
	vector<int> b;
	//For O(log n) search:
	//vector<int> where;
	//vector<int> rig;
	node *l = NULL, *r = NULL;
};

node* build(int *lpos, int *rpos, int clo, int chi){
	node* v = new node;
	v->lo = clo;
	v->hi = chi;
	if(clo == chi || rpos <= lpos)return v;
	int mid = (clo+chi)/2;
	int la = 0;
	v->b.reserve(rpos - lpos);
	for(auto i = lpos; i != rpos; i++){
		if(*i <= mid)la++;
		v->b.push_back(la);
	}
	/*
	For O(log n) search:
	v->rig.resize(v->b.size()+1);
	v->rig.assign(v->rig.size(), -1);
	v->where.resize(v->b.size()+1);
	v->where.assign(v->where.size(), -1);
	for(int i = 0; i < (int)v->b.size(); i++){
		if(!(i > 0 && v->b[i] == v->b[i - 1]))v->where[v->b[i]] = i;
		if(!(i > 0 && (i + 1 - v->b[i]) == (i - 1 + 1 - v->b[i - 1])))v->rig[i + 1 - v->b[i]] = i;
	}*/
	auto split = stable_partition(lpos, rpos, [mid](int i){return i <= mid;});
	v->l = build(lpos, split, clo, mid);
	v->r = build(split, rpos, mid + 1, chi);
	return v;
}

int kth_element(node *v, int l, int r, int k){
	if(l > r)return inf;
	if(v->lo == v->hi)return v->lo;
	int cnt = v->b[r] - ((l == 0) ? 0 : v->b[l - 1]);
	if(k <= cnt)return kth_element(v->l, (l==0) ? 0 : v->b[l-1], v->b[r] - 1, k);
	else return kth_element(v->r, l - ((l==0) ? 0 : v->b[l-1]), r - v->b[r], k - cnt);
}

int number_of_occurences(node *v, int pref, int what){
	if(pref == -1)return 0;
	if(v->lo == v->hi)return pref + 1;
	int mid = (v->lo + v->hi)/2;
	if(what <= mid)return number_of_occurences(v->l, v->b[pref]-1, what);
	else return number_of_occurences(v->r, (pref+1-v->b[pref])-1, what);
}

/*
O(log n) search
int get_by_num(node *v, int cnt, int what){
	if(v->lo == v->hi)return cnt - 1;
	int mid = (v->lo + v->hi)/2;
	int prv;
	if(what <= mid){
		prv = get_by_num(v->l, cnt, what);
		if(prv == -1)return -1;
		prv++;
		return v->where[prv];
	}
	else{
	   	prv = get_by_num(v->r, cnt, what);
		if(prv== -1)return -1;
		prv++;
		return v->rig[prv];
	}
}*/

void upd_adjswap(node *v, int i){
	if(v->lo == v->hi)return;
	bool kek = v->b[i] - ((i == 0) ? 0 : v->b[i - 1]) == 1, bishkek = v->b[i + 1] - v->b[i] == 1;
	if(kek == bishkek){
		if(kek)upd_adjswap(v->l, v->b[i]-1);
		else upd_adjswap(v->r, (i+1-v->b[i])-1);
		return;
	}
	if(kek){
		//Needed for O(log n) search
		//if(v->where[v->b[i]] == i)v->where[v->b[i]]++; if(v->rig[i + 1 - v->b[i] + 1] > i)v->rig[i + 1 - v->b[i] + 1] = i;
		v->b[i]--;
	}
	else {
		//Needed for O(log n) search
		//if(v->where[v->b[i] + 1] > i)v->where[v->b[i] + 1] = i; if(v->rig[i + 1 - v->b[i]] == i)v->rig[i + 1 - v->b[i]]++; 
		v->b[i]++;
	}
}

int n, q;
set<int> ayy;
map<int, int> lmao;

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d%d", &n, &q);
	F(i, 0, n)scanf("%d", &a[i]);
	F(i, 0, n)ayy.ins(a[i]);
	int cid = 0;
	I(c, ayy)lmao[c] = cid++;
	F(i, 0, n)a[i] = lmao[a[i]];
	F(i, 0, n)lel[i] = a[i];
	node * root = build(lel, lel + n, 0, cid + 1);
	F(i, 0, q){
		int tp;
		scanf("%d", &tp);
		if(tp == 0){
			int pos, l, k;
			scanf("%d%d%d", &pos, &l, &k);
			int kek = kth_element(root, 0, pos, k);
			int lb = 0, rb = n - 1;
			if(number_of_occurences(root, n - 1, kek) < l){printf("-1\n"); continue;}
			while(rb - lb > 1){
				int mid = (lb + rb) >> 1;
				if(number_of_occurences(root, mid, kek) >= l)rb = mid;
				else lb = mid + 1;
			}
			if(number_of_occurences(root, lb, kek) >= l)rb = lb;
			printf("%d\n", rb);
			//O(log n) search:
			//int res = get_by_num(root, l, kek);
			//printf("%d\n", res);
		}
		else{
			int pos;
			scanf("%d", &pos);
			if(pos == n - 1)continue;
			upd_adjswap(root, pos);
			swap(a[pos], a[pos + 1]);
		}
	}
	return 0;
}
