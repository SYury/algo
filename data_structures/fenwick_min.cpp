#include<bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int inf = 1e9 + 7;

int t[2][MAXN];
int a[MAXN];
int n, q;

int get(int l, int r){
	if(l > r)return n + 1;
	int ans = n + 1;
	int v = l;
	int last = -1;
	while(v <= r){
		if(last != -1 && a[ans] > a[t[1][last]])ans = t[1][last];
		last = v;
		v += v & -v;
	}
	if(a[ans] > a[last])ans = last;
	v = r;
	last = -1;
	while(v >= l){
		if(last != -1 && a[ans] > a[t[0][last]])ans = t[0][last];
		last = v;
		v -= v & -v;
	}
	return ans;
}

void upd(int pos, int x){
	int v = pos;
	int l = pos - 1, r = pos + 1;
	int minl = n + 1, minr = n + 1;
	a[pos] = x;
	while(v <= n){
		if(t[0][v] != pos){
			if(a[t[0][v]] > x)t[0][v] = pos;
		}
		else{
			while(l > 0 && l >= v - (v & -v) + 1){if(a[minl] > t[1][l])minl = t[1][l]; l -= l & -l;}
			while(r < n && r <= v){if(minr > t[0][r])minr = t[0][r]; r += r & -r;}
			int cmin = min({a[minl], a[minr], x});
			if(cmin == minl)t[0][v] = minl;
			if(cmin == minr)t[0][v] = minr;
			if(cmin == x)t[0][v] = pos;
			
		}
		v += v & -v;
	}
	v = pos;
	l = pos - 1, r = pos + 1;
	minl = n + 1, minr = n + 1;
	while(v > 0){
		if(t[1][v] != pos){
			if(a[t[1][v]] > x)t[1][v] = pos;
		}
		else{
			while(l > 0 && l >= v){if(a[minl] > t[1][l])minl = t[1][l]; l -= l & -l;}
			while(r < n && r <= v + (v & -v) - 1){if(a[minr] > t[0][r])minr = t[0][r]; r += r & -r;}

			int cmin = min({a[minl], a[minr], x});
			if(cmin == minl)t[1][v] = minl;
			if(cmin == minr)t[1][v] = minr;
			if(cmin == x)t[1][v] = pos;
		}
		v -= v & -v;
	}
}

int main(){
	freopen("sparse.in", "r", stdin);
	freopen("sparse.out", "w", stdout);
	scanf("%d%d", &n, &q);
	a[n + 1] = inf;
	for(int i = 1; i <= n; i++){a[i] = inf; t[0][i] = t[1][i] = i;}
	for(int i = 1; i <= n; i++){if(i == 1){scanf("%d", &a[i]);}else{a[i] = (23 * a[i - 1] + 21563)%16714589;} upd(i, a[i]);}
	int l, r;
	int ol, orr;
	int lans = 0;
	for(int i = 0; i < q; i++){
		//int l, r;
		if(i == 0){scanf("%d%d", &l, &r);}
		else{l = 1 + (17 * ol + 751 + lans + i + i)%n; r = 1 + (13 * orr + 593 + lans + 5 * i)%n;}
		ol = l; orr = r;
		if(l > r)swap(l, r);
		lans = a[get(l, r)];
		if(i == q - 1)printf("%d %d %d", ol, orr, lans);
		
	}
	return 0;
}
