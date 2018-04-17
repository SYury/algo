typedef double fftdbl;

const fftdbl fftpi = acos(-1.0);

struct base{
	fftdbl re, im;
	base(){}
	base(fftdbl _re, fftdbl _im):re(_re), im(_im){}
	base operator + (const base & a)const{return base(re + a.re, im + a.im);}
	base operator - (const base & a)const{return base(re - a.re, im - a.im);}
	base operator * (const base & a)const{return base(re * a.re - im * a.im, re * a.im + im * a.re);}
	base operator / (const fftdbl & val)const{return base(re / val, im / val);}
};

vector<base> root_pw[30];
vector<base> root_inv_pw[30];

void FFT_prepare(int max_lg){
	for(int i = max_lg; i >= 1; i--){
		root_pw[i].resize(1<<(i - 1));
		fftdbl ang = 2 * fftpi / (1<<i);
		base root(cos(ang), sin(ang));
		base w(1, 0);
		for(int j = 0; j < (int)root_pw[i].size(); j++){
			root_pw[i][j] = w;
			w = w * root;
		}
	}
	for(int i = max_lg; i >= 1; i--){
		root_inv_pw[i].resize(1<<(i - 1));
		fftdbl ang = -2 * fftpi / (1<<i);
		base root(cos(ang), sin(ang));
		base w(1, 0);
		for(int j = 0; j < (int)root_inv_pw[i].size(); j++){
			root_inv_pw[i][j] = w;
			w = w * root;
		}
	}
}

void FFT(base * from, base * to, bool inv){
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
		vector<base> & w = inv ? root_inv_pw[lg] : root_pw[lg];
		for(int i = 0; i < n; i += len){
			base *l = from + i, *r = from + i + len/2;
			for(int j = 0; j < len/2; j++){
				base u = *l, v = *r * w[j];
				*l = u + v;
				*r = u - v;
				++l; ++r;
			}
		}
	}
	if(inv)
		for(base * curr = from; curr != to; curr++)
			*curr = *curr / n;
}

