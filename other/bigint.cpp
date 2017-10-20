namespace FFT{
	const long double PI = 3.1415926535897;
	struct complex{
		long double re = 0.0, im = 0.0;
		complex(){re = im = 0.0;}
		complex(const long double & x){re = x; im = 0.0;}
		complex(const long double & x, const long double & y){re = x; im = y;}
		void operator /= (const long double & x){re /= x; im /= x;}
	};
	const complex ZERO = complex(0.0);
	const complex ONE = complex(1.0);
	complex operator + (const complex & a, const complex & b){return complex(a.re + b.re, a.im + b.im);}
	complex operator - (const complex & a, const complex & b){return complex(a.re - b.re, a.im - b.im);}
	complex operator * (const complex & a, const complex & b){return complex(a.re * b.re - a.im * b.im, a.re * b.im + b.re * a.im);}
	typedef std::vector<complex> poly;
	int rev(int x, int sz){
		int ans = 0;
		for(int i = 0; i < sz; i++){
			if(x & (1 << i))
				ans |= 1 << (sz - 1 - i);
		}
		return ans;
	}
	void fft(poly & a, bool is_inverse){
		int n = (int)a.size();
		int lg = 0;
		while((1<<lg)<n)lg++;
		for(int i = 0; i < n; i++)if(i < rev(i, lg))std::swap(a[i], a[rev(i, lg)]);
		for(int len = 2; len <= n; len <<= 1){
			long double phi = 2 * PI / len;
			if(is_inverse)phi = -phi;
			complex base_w(cos(phi), sin(phi));
			for(int i = 0; i < n; i += len){
				complex w = ONE;
				complex t;
				auto pu = a.begin() + i, pv = a.begin() + i + len/2, pue = a.begin() + i + len/2;
				for(; pu != pue; ++pu, ++pv){
					t = *pv * w;
					*pv = *pu - t;
					*pu = *pu + t;
					w = w * base_w;
				}
			}
		}
		if(is_inverse)for(int i = 0; i < n; i++){a[i] /= n;}
		
	}
}; // FFT

struct BigInteger : public std::vector<int>{
	//std::vector<int> a;
	bool negative;
	int length()const{return this->size();}
	bool is_zero()const{
		return (this->length() == 1 && (*this)[0] == 0);
	}
	BigInteger(){
		this->negative = false;
		this->resize(1);
		(*this)[0] = 0;
	}
	BigInteger (const int & y){
		int x = y;
		if(x < 0){this->negative = true; x = -x;}else this->negative = false;
		this->clear();
		if(x == 0){this->push_back(0);}
		while(x){
			this->push_back(x%10);
			x /= 10;
		}
	}
	BigInteger& operator = (const int & y){
		int x = y;
		if(x < 0){this->negative = true; x = -x;}else this->negative = false;
		this->clear();
		if(x == 0){this->push_back(0); return *this;}
		while(x){
			this->push_back(x%10);
			x /= 10;
		}
		return *this;
	}
	explicit operator bool()const;
	//operator bool(){return !this->is_zero();}
	std::string toString() const{
		std::string ans = "";	
		for(auto c : *this)ans += c + '0';
		ans += (this->negative) ? "-" : "";
		std::reverse(ans.begin(), ans.end());
		return ans;
	}
	BigInteger abs() const{
		BigInteger res = *this;
		res.negative = false;
		return res;
	}
	/*int operator [] (const int & i)const{return a[i];}
	void push_front(const int & x){this->a.push_front(x);}
	void push_back(const int & x){this->a.push_back(x);}
	void pop_front(){this->a.pop_front();}
	void pop_back(){this->a.pop_back();}*/
	BigInteger operator - (void){
		if(this->is_zero())return *this;
		BigInteger tmp = *this;
		tmp.negative = !tmp.negative;
		return tmp;
	}
	friend std::ostream& operator <<(std::ostream & os, const BigInteger & x){
		os << x.toString();
		return os;
	}
	friend std::istream& operator >>(std::istream & is, BigInteger & x){
		std::string tmp;
		is >> tmp;
		x.clear();
		if(tmp[0] == '-'){tmp.erase(tmp.begin()); x.negative = true;}
		else x.negative = false;
		x.resize(tmp.length());
		for(size_t i = 0; i < tmp.length(); i++)x[i] = tmp[tmp.length() - i - 1] - '0';
		if(x.is_zero())x.negative = false;
		return is;
	}
};
	bool operator == (const BigInteger & y, const BigInteger & x){
		if(y.negative && !x.negative)return false;
		if(!y.negative && x.negative)return false;
		if(y.length() != x.length())return false;
		for(int i = 0; i < y.length(); i++)if((y)[i] != x[i])return false;
		return true;
	}
	bool operator != (const BigInteger & y, const BigInteger & x){return !(y == x);}
	bool operator < (const BigInteger & y, const BigInteger & x){
		if(y.negative && !x.negative)return true;
		if(!y.negative && x.negative)return false;
		bool true_val = true;
		if(y.negative)true_val = false;
		if(y.length() < x.length())return true_val;
		if(y.length() > x.length())return !true_val;
		for(int i = (int)y.length() - 1; i >= 0; i--)if((y)[i] < x[i])return true_val; else if((y)[i] > x[i]){return !true_val;}
		return false;
	}
	bool operator <= (const BigInteger & y, const BigInteger & x){return (y < x) || (y == x);}
	bool operator > (const BigInteger & y, const BigInteger & x) {
		return !(y <= x);
	}
	bool operator >= (const BigInteger & y, const BigInteger & x) {
		return !(y < x);
	}
	BigInteger operator + (const BigInteger & y, const BigInteger & x);
	BigInteger operator - (const BigInteger & y, const BigInteger & x);
BigInteger operator + (const BigInteger & y, const BigInteger & x) {
	BigInteger ans;
	ans.clear();
	if(y.negative == x.negative){
		ans.negative = x.negative;
		int car = 0;
		for(int i = 0; i < (int)std::max(y.length(), x.length()); i++){
			int l = (i < y.length()) ? y[i] : 0;
			int r = (i < x.length()) ? x[i] : 0;
			car += l + r;
			ans.push_back(car%10);
			car /= 10;
		}
		while(car){ans.push_back(car%10); car /= 10;}
		while(ans.length() > 1 && ans.back() == 0)ans.pop_back();
	}
	else{
		if(y.negative){
			ans = x - y.abs();
		}
		else{
			ans = y - x.abs();
		}
	}
	return ans;
}
BigInteger operator - (const BigInteger &y, const BigInteger & x){
	BigInteger ans;
	ans.clear();
	if(y.negative == x.negative){
		if(x.abs() > y.abs())ans.negative = !x.negative;
		else ans.negative = x.negative;
		const BigInteger *big = &y, *small = &x;
		if(y.abs() < x.abs())std::swap(big, small);
		bool take = false;
		for(int i = 0; i < (int)std::max(y.length(), x.length()); i++){
			int l = (*big)[i];
			int r = (small->length() <= i) ? 0 : (*small)[i];
			l -= r;
			if(take)l--;
			take = false;
			if(l < 0){l += 10; take = true;}
			ans.push_back(l);
		}
	}
	else{
		if(y.negative){ans = y.abs() + x; ans.negative = true;}
		else{ans = y + x.abs();}
	}
	while(ans.length() > 1 && ans.back() == 0)ans.pop_back();
	if(ans.is_zero())ans.negative = false;
	return ans;
}
	BigInteger operator * (const BigInteger & y, const BigInteger & x) {
		FFT::poly pa, px;
		int n = y.length(), m = x.length();
		int lgn = 0, lgm = 0;
		while((1<<lgn)<n)lgn++;
		n = 1<<lgn;
		while((1<<lgm)<m)lgm++;
		m = 1<<lgm;
		n = m = 2*std::max(n, m);
		pa.resize(n);
		for(int i = 0; i < n; i++){
			if(i < y.length())pa[i] = FFT::complex(y[i]); else pa[i] = FFT::ZERO;
		}
		px.resize(m);
		for(int i = 0; i < m; i++){
			if(i < x.length())px[i] = FFT::complex(x[i]); else px[i] = FFT::ZERO;
		}
		FFT::fft(pa, false);
		FFT::fft(px, false);
		for(int i = 0; i < n; i++)pa[i] = pa[i] * px[i];
		FFT::fft(pa, true);
		std::vector<int> res(n);
		for(int i = 0; i < n; i++)res[i] = (int)(pa[i].re + 0.5);
		int car = 0;
		bool not_zero = false;
		for(int i = 0; i < n; i++)if(res[i] != 0){not_zero = true; break;}
		if(!not_zero)return BigInteger(0);
		BigInteger ans;
		ans.negative = y.negative ^ x.negative;
		for(int i = 0; i < (int)res.size(); i++){
			res[i] += car;
			car = res[i]/10;
			res[i] %= 10;
		}
		while(car){
			res.push_back(car%10);
			car /= 10;
		}
		while(res.size() > 1 && res.back() == 0)res.pop_back();
		ans.resize(res.size());
		for(int i = 0; i < (int)res.size(); i++)ans[i] = res[i];
		return ans;
	}
	BigInteger operator / (const BigInteger & y, const BigInteger & x) {
		if(x.is_zero())exit(123);
		if(y.is_zero())return y;
		BigInteger ans, p = y.abs(), q;
		ans.resize(1); ans[0] = 0;
		while(true){
			q = x.abs();
			int pw = 0;
			while(q <= p){
				q.emplace(q.begin(), 0);
				pw++;
			}
			pw--;
			if(pw == -1)break;
			q.erase(q.begin());
			p = p - q;
			BigInteger add;
			add.resize(pw + 1);
			for(int i = 0; i < pw; i++)add[i] = 0;
			add[pw] = 1;
			ans = ans + add;
		}
		ans.negative = y.negative ^ x.negative;
		if(ans.is_zero())ans.negative = false;
		return ans;
	}
	BigInteger operator % (const BigInteger & y, const BigInteger & x) {
		BigInteger z = y/x;
		return y - (z*x);
	}
	BigInteger& operator += (BigInteger & y, const BigInteger & x){
		y = y + x;
		return y;
	}
	BigInteger& operator -= (BigInteger & y, const BigInteger & x){
		y = y - x;
		return y;
	}
	BigInteger& operator *= (BigInteger & y, const BigInteger & x){
		y = y * x;
		return y;
	}
	BigInteger& operator /= (BigInteger & y, const BigInteger & x){
		y = y / x;
		return y;
	}
	BigInteger& operator %= (BigInteger & y, const BigInteger & x){
		y = y % x;
		return y;
	}
	BigInteger& operator --(BigInteger & x){
		x = x - BigInteger(1);
		return x;
	}
	BigInteger operator --(BigInteger & x, int){
		BigInteger res = x;
		x = x - BigInteger(1);
		return res;
	}
	BigInteger& operator ++(BigInteger & x){
		x = x + BigInteger(1);
		return x;
	}
	BigInteger operator ++(BigInteger & x, int){
		BigInteger res = x;
		x = x + BigInteger(1);
		return res;
	}
BigInteger::operator bool() const{
	return !this->is_zero();
}
