#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

template<int T>
struct sat2{
	bool gr[T][T];
	bool grt[T][T];
	int comp[T];
	int order[T];
	int n;
	int ord_ptr;
	sat2(){}
	void init(int _n){
		n = _n; ord_ptr = 0;
		for(int i = 0; i < n; i++){
			comp[i] = 0;
			for(int j = i; j < n; j++){
				gr[i][j] = gr[j][i] = false;
				grt[i][j] = grt[j][i] = false;
			}
		}
	}
	void add_edge(int v, int u){
		gr[v][u] = true;
		grt[u][v] = true;
	}
	void dfs(int v){
		comp[v] = -1;
		for(int i = 0; i < n; i++)if(comp[i] == 0 && gr[v][i])dfs(i);
		order[ord_ptr++] = v;
	}
	void dfs1(int v, int curr){
		comp[v] = curr;
		for(int i = 0; i < n; i++)if(comp[i] == -1 && grt[v][i])dfs1(i, curr);
	}
	void solve(){
		for(int i = 0; i < n; i++)if(comp[i] == 0)dfs(i);
		reverse(order, order + n);
		int ptr = 0;
		for(int i = 0; i < n; i++)if(comp[order[i]] == -1)dfs1(order[i], ptr++);
	}
};

sat2<222> solver;
int n;
int a[111][111];
int b[111][111];
bool good[111];

int main(){
	scanf("%d", &n);
	solver.init(2*n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++){
			scanf("%d", &a[i][j]);
			if(i <= j || a[i][j] == -1)continue;
			if(a[j][i] == -1)continue;
			int ti = 2 * i, tj = 2 * j;
			if(a[i][j] == 0 && a[j][i] == 0){
				solver.add_edge(ti, tj^1);
				solver.add_edge(tj, ti^1);
			}
			if(a[i][j] == 1 && a[j][i] == 0){
				solver.add_edge(ti^1, tj^1);
				solver.add_edge(tj, ti);
			}
			if(a[i][j] == 0 && a[j][i] == 1){
				solver.add_edge(ti, tj);
				solver.add_edge(tj^1, ti^1);
			}
			if(a[i][j] == 1 && a[j][i] == 1){
				solver.add_edge(ti^1, tj);
				solver.add_edge(tj^1, ti);
			}
		}
	solver.solve();
	for(int i = 0; i < n; i++){
		if(solver.comp[2*i] == solver.comp[2*i + 1]){printf("Impossible"); return 0;}
		good[i] = solver.comp[2*i] > solver.comp[2*i + 1];
		for(int j = 0; j < i; j++){
			if(a[i][j] == -1 && a[j][i] == -1){b[i][j] = b[j][i] = 1; continue;}
			if(a[i][j] == 0 && good[i]){b[i][j] = 0; b[j][i] = 3; continue;}
			if(a[i][j] == 1 && !good[i]){b[i][j] = 0; b[j][i] = 3; continue;}
			if(a[j][i] == 0 && good[j]){b[j][i] = 0; b[i][j] = 3; continue;}
			if(a[j][i] == 1 && !good[j]){b[j][i] = 0; b[i][j] = 3; continue;}
			b[i][j] = b[j][i] = 1;
		}
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)printf("%d ", b[i][j]);
		printf("\n");
	}
	return 0;
}
