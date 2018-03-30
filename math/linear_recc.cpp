template<typename T, size_t MAXN>
class LinearRecurrence{
//  A_i = C_0 * A_i-1 + ... + C_N-1 * A_i-N
//  T should support +=, *, =, conversion from int
	int N;
	T C[MAXN][MAXN];
	T matr[MAXN][MAXN];
	T start[MAXN];
	T tmp[MAXN][MAXN];
	T buf[MAXN][MAXN];
	T tmr[MAXN];
	public:
	LinearRecurrence();
	LinearRecurrence(int _N, T* coeff, T* _start){
		N = _N;
		for(int i = 0; i + 1 < N; i++){
			for(int j = 0; j < N; j++)
				C[i][j] = 0;
			C[i][i + 1] = 1;
		}
		for(int j = 0; j < N; j++)
			C[N - 1][j] = coeff[N - 1 - j];
		for(int i = 0; i < N; i++)
			start[i] = _start[i];
	}
	void mul(T (&A)[MAXN][MAXN], T (&B)[MAXN][MAXN], T (&result)[MAXN][MAXN]){
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				result[i][j] = 0;
				for(int k = 0; k < N; k++)
					result[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	void mulc(T (&A)[MAXN][MAXN], T (&col)[MAXN], T (&result)[MAXN]){
		for(int i = 0; i < N; i++)
			result[i] = 0;
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				result[i] += A[i][j] * col[j];
			}
		}
	}
	T nth(unsigned long long n){
		if(n < (unsigned)N)return start[n];
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				tmp[i][j] = C[i][j];
				matr[i][j] = i == j ? 1 : 0;
			}
		}
		while(n){
			if(n&1){
				mul(matr, tmp, buf);
				for(int i = 0; i < N; i++)
					for(int j = 0; j < N; j++)
						matr[i][j] = buf[i][j];
			}
			mul(tmp, tmp, buf);
			for(int i = 0; i < N; i++)
				for(int j = 0; j < N; j++)
					tmp[i][j] = buf[i][j];
			n >>= 1;
		}
		mulc(matr, start, tmr);
		return tmr[0];
	}
};
