const long double eps = 1e-8;

int gauss(vector<vector<long double> > a, vector<long double> & ans){
	int n = (int)a.size();
	int m = (int)a[0].size() - 1;
	vector<int> where(m, -1);
	for(int x = 0, y = 0; x < n && y < m; y++){
		int kek = x;
		for(int i = x; i < n; i++){
			if(abs(a[i][y]) > abs(a[kek][y]))kek = i;
		}
		if(abs(a[kek][y]) < eps)continue;
		for(int i = y; i <= m; i++)
			swap(a[kek][i], a[x][i]);
		where[y] = x;
		for(int i = 0; i < n; i++){
			if(i == x)continue;
			long double c = a[i][y] / a[x][y];
			for(int j = y; j <= m; j++)
				a[i][j] -= a[x][j] * c;
		}
		++x;
	}
	ans.resize(m);
	ans.assign(m, 0);
	for(int i = 0; i < m; i++){
		if(where[i] != -1)
			ans[i] = a[where[i]][m] / a[where[i]][i];
	}
	for(int i = 0; i < n; i++){
		long double sum = 0;
		for(int j = 0; j < m; j++)
			sum += ans[j] * a[i][j];
		if(abs(sum - a[i][m]) > eps)
			return 0;
	}
	for(int i = 0; i < m; i++){
		if(where[i] == -1)
			return -1;
	}
	return 1;
}

template<int MAXN>
int gauss2(vector<bitset<MAXN> > a, bitset<MAXN> & ans, int m){
	int n = (int)a.size();
	vector<int> where(m, -1);
	for(int x = 0, y = 0; y < m && x < n; ++y){
		for(int i = x; i < n; i++){
			if(a[i][y]){
				swap(a[i], a[x]);
				break;
			}
		}
		if(!a[x][y])
			continue;
		where[y] = x;
		for(int i = 0; i < n; i++){
			if(i != x && a[i][y])
				a[i] ^= a[x];
		}
		++x;
	}
	ans.reset();
	for(int i = 0; i < m; i++){
		if(where[i] != -1)
			ans[i] = a[where[i]][m];
	}
	for(int i = 0; i < n; i++){
		int sum = 0;
		for(int j = 0; j < m; j++)
			sum ^= ans[j] && a[i][j];
		if(sum != a[i][m])
			return 0;
	}
	for(int i = 0; i < m; i++){
		if(where[i] == -1)
			return -1;
	}
	return 1;
}
