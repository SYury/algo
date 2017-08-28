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

const int MAXN = (1<<11) + 3;

lint t[MAXN][MAXN];
int tt, n;
lint a[MAXN][MAXN];

lint get(int x, int y){
	if(x == 0 || y == 0)ret 0;
	lint ans = 0;
	while(x > 0){
		int cy = y;
		while(cy > 0){
			ans += t[x][cy];
			cy -= cy&-cy;
		}
		x -= x&-x;
	}
	ret ans;
}

lint get(int x1, int y1, int x2, int y2){
	ret get(x2, y2) + get(x1 - 1, y1 - 1) - get(x1 - 1, y2) - get(x2, y1 - 1);
}

void upd(int x, int y, lint nval){
	int old = x;
	while(x <= n){
		int cy = y;
		while(cy <= n){
			t[x][cy] += nval - a[old][y];
			cy += cy&-cy;
		}
		x += x&-x;
	}
	a[old][y] = nval;
}

int main(){
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	scanf("%d", &tt);
	F(g, 0, tt){
		scanf("%d", &n);
		F(i, 1, n + 1)
			F(j, 1, n + 1)
				t[i][j] = a[i][j] = 0;
		while(true){
			char c = getc(stdin);
			while(!isalpha(c))c = getc(stdin);
			if(c == 'E'){while(isalpha(c))c = getc(stdin); brk;}
			c = getc(stdin);
			bool kek = c == 'E';
			while(isalpha(c))c = getc(stdin);
			if(kek){int x, y; lint d; scanf("%d%d%lld", &x, &y, &d); upd(x + 1, y + 1, d);}
			else{
				int x1, y1, x2, y2;
				scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
				printf("%lld\n", get(x1 + 1, y1 + 1, x2 + 1, y2 + 1));
			}
		}
		printf("\n");
	}
	ret 0;
}

