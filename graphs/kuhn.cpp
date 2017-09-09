#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

const int MAXN = 1e5 + 5;

int match[MAXN];
int used[MAXN];
bool was[MAXN];
vector<int> gr[MAXN];
int n;
int cc;

bool dfs(int v){
	if(used[v] == cc)return false;
	used[v] = cc;
	for(auto u : gr[v]){
		if(match[u] == -1){
			was[v] = true;
			match[u] = v;
			return true;
		}

	}
	for(auto u : gr[v]){
		if(dfs(match[u])){
			was[v] = true;
			match[u] = v;
			return true;
		}
	}
	return false;

}

void kuhn(){
	cc = 0;
	for(int i = 0; i < n; i++){used[i] = 0; was[i] = false; match[i] = -1;}
	for(int run = 1; run;){
		cc++;
		run = 0;
		for(int i = 0; i < n; i++)used[i] = false;
		for(int i = 0; i < n; i++){
			if(!was[i] && dfs(i))run = 1;
		}
	}
}

int w, h, a, b;
char field[333][333];

int main(){
	scanf("%d%d%d%d", &h, &w, &a, &b);
	n = w * h;
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			char c = getc(stdin);
			while(c != '.' && c != '*')c = getc(stdin);
			field[i][j] = c;
			if(field[i][j] == '.')continue;
			if(i > 0 && field[i - 1][j] == '*'){
				int l = i * w + j, r = (i - 1) * w + j;
				gr[l].push_back(r); gr[r].push_back(l);
			}
			if(j > 0 && field[i][j - 1] == '*'){
				int l = i * w + j, r = i * w + j - 1;
				gr[l].push_back(r); gr[r].push_back(l);
			}
		}
	}
	if(2 * b <= a){
		int ans = 0;
		for(int i = 0; i < h; i++)for(int j = 0; j < w; j++)if(field[i][j] == '*')ans += b;
		printf("%d", ans);
		return 0;
	}
	kuhn();
	int ans = 0, ex = 0;
	for(int i = 0; i < n; i++){
		int x = i/w, y = i%w;
		if(field[x][y] == '.')continue;
		if(match[i] == -1)ex += b; else ans += a;
	}
	printf("%d", ans/2 + ex);
	return 0;
}
