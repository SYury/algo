const int N = 505;
const int mod = 998244353;

inline int inv(long long x){
    long long res = 1;
    long long pw = mod - 2;
    while(pw){
        if(pw&1)
            res = res * x % mod;
        x = x * x % mod;
        pw >>= 1;
    }
    return res;
}

inline void iadd(int& x, int y){
    x += y;
    x -= x >= mod ? mod : 0;
}

inline int add(int x, int y){
    iadd(x, y);
    return x;
}

inline void isub(int& x, int y){
    x -= y;
    x += x < 0 ? mod : 0;
}

inline int sub(int x, int y){
    isub(x, y);
    return x;
}

int a[N][N];
int p[N][N];
int n;

inline void hessenberg(){
    for(int j = 0; j < n - 2; j++){
        int pos = -1;
        for(int i = j + 2; i < n; i++)
            if(a[i][j]){
                pos = i; break;
            }
        if(pos == -1)
            continue;
        if(!a[j + 1][j]){
            for(int k = 0; k < n; k++)
                swap(a[pos][k], a[j + 1][k]);
            for(int k = 0; k < n; k++)
                swap(a[k][pos], a[k][j + 1]);
        }
        int mul = inv(a[j + 1][j]);
        for(int k = j + 2; k < n; k++){
            int u = a[k][j] * 1ll * mul % mod;
            for(int t = 0; t < n; t++)
                isub(a[k][t], u * 1ll * a[j + 1][t] % mod);
            for(int t = 0; t < n; t++)
                iadd(a[t][j + 1], u * 1ll * a[t][k] % mod);
        }
    }
}

inline void calc_poly(){
    p[0][0] = 1;
    for(int k = 0; k < n; k++){
        for(int i = 1; i <= n; i++)
            p[k + 1][i] = p[k][i - 1];
        for(int i = 0; i <= n; i++)
            isub(p[k + 1][i], p[k][i] * 1ll * a[k][k] % mod);
        int t = 1;
        for(int i = 1; i <= k; i++){
            t = t * 1ll * a[k - i + 1][k - i] % mod;
            for(int c = 0; c <= n; c++)
                isub(p[k + 1][c], (t * 1ll * a[k - i][k] % mod) * 1ll * p[k - i][c] % mod);
        }
    }
}

inline int eval_poly(int x){
    int ans = 0;
    int pw = 1;
    for(int i = 0; i <= n; i++){
        iadd(ans, pw * 1ll * p[n][i] % mod);
        pw = pw * 1ll * x % mod;
    }
    return ans;
}
