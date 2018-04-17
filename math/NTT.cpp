const long long mod = (119<<23) + 1;
const long long maxpw = 1<<23;

inline long long powmod(long long x, long long pw){
	long long res = 1;
	while(pw){
		if(pw&1)res = res * x % mod;
		x = x * x % mod;
		pw >>= 1;
	}
	return res;
}

const long long root = powmod(3, 119);
const long long root_inv = powmod(root, mod - 2);

//also 5 * 2^25 + 1, 3^5 and 7 * 2^26 + 1, 3^7

vector<long long> root_pw[30];
vector<long long> root_inv_pw[30];

void NTT_prepare(int max_lg){
	long long wn = root;
	for(int i = max_lg; i < __builtin_ctz(maxpw); i++)
		wn = wn * wn % mod;
	for(int i = max_lg; i >= 1; i--){
		root_pw[i].resize(1<<(i - 1));
		long long w = 1;
		for(int j = 0; j < (int)root_pw[i].size(); j++){
			root_pw[i][j] = w;
			w = w * wn % mod;
		}
		wn = wn * wn % mod;
	}
	wn = root_inv;
	for(int i = max_lg; i < __builtin_ctz(maxpw); i++)
		wn = wn * wn % mod;
	for(int i = max_lg; i >= 1; i--){
		root_inv_pw[i].resize(1<<(i - 1));
		long long w = 1;
		for(int j = 0; j < (int)root_inv_pw[i].size(); j++){
			root_inv_pw[i][j] = w;
			w = w * wn % mod;
		}
		wn = wn * wn % mod;
	}
}

void NTT(long long * from, long long * to, bool inv){
	int n = to - from;
	int lg = __builtin_ctz(n);
	for(int i = 0; i < n; i++){
		int rev = 0;
		int k = lg - 1;
		for(int j = 0; j < lg; j++){
			rev |= i&(1<<j) ? 1<<k : 0;
			--k;
		}
		if(i < rev)swap(*(from + i), *(from + rev));
	}
	lg = 0;
	for(int len = 2; len <= n; len <<= 1){
		++lg;
		vector<long long> & w = inv ? root_inv_pw[lg] : root_pw[lg];
		for(int i = 0; i < n; i += len){
			long long *l = from + i, *r = from + i + len/2;
			for(int j = 0; j < len/2; j++){
				long long u = *l, v = *r * w[j] % mod;
				*l = u + v;
				*r = u - v;
				*l -= *l >= mod ? mod : 0;
				*r += *r < 0 ? mod : 0;
				++l; ++r;
			}
		}
	}
	if(inv){
		long long ni = powmod(n, mod - 2);
		for(long long * curr = from; curr != to; curr++)
			*curr = *curr * ni % mod;
	}
}
