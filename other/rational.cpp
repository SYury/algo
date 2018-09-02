
//denominator should be > 0
template<typename T>
class Rational{
#define NORMALIZED_OPERATIONS 1
public:
	T p, q;
	inline static T gcd(const T & a, const T & b){
		return a == 0 ? b : gcd(b%a, a);
	}
	inline static T abs(const T & a){
		return a < 0 ? -a : a;
	}
	Rational(){};
	Rational(const T & pp, const T & qq):p(pp), q(qq){}
	Rational (const T & x):p(x), q(1){}
	Rational& operator = (const T & x){
		p = x;
		q = 1;
		return *this;
	}
	inline char sgn()const{
		return p == 0 ? 0 : (p < 0 ? -1 : 1);
	}
	explicit operator bool()const{
		return this->sgn() == 0;
	}
	friend std::ostream& operator <<(std::ostream & os, const Rational & x){
		os << x.p << '/' << x.q;
		return os;
	}
	inline void normalize_self(){
		if(q < 0){p = -p; q = -q;}
		T g = gcd(abs(p), q);
		p /= g; q /= g;
	}
	inline Rational normalize()const{
		Rational res = *this;
		res.normalize_self();
		return res;
	}
	inline Rational& operator += (const Rational & r){
		T pp = this->p * r.q + this->q * r.p;
		T qq = this->q * r.q;
		this->p = pp;
		this->q = qq;
		#ifdef NORMALIZED_OPERATIONS
		this->normalize_self();
		#endif
		return *this;
	}
	inline Rational& operator -= (const Rational & r){
		T pp = this->p * r.q - this->q * r.p;
		T qq = this->q * r.q;
		this->p = pp;
		this->q = qq;
		#ifdef NORMALIZED_OPERATIONS
		this->normalize_self();
		#endif
		return *this;
	}
	inline Rational& operator *= (const Rational & r){
		this->p *= r.p;
		this->q *= r.q;
		#ifdef NORMALIZED_OPERATIONS
		this->normalize_self();
		#endif
		return *this;
	}
	inline Rational& operator /= (const Rational & r){
		this->p *= r.q;
		this->q *= r.p;
		if(this->q < 0){this->q = -this->q; this->p = -this->p;}
		#ifdef NORMALIZED_OPERATIONS
		this->normalize_self();
		#endif
		return *this;
	}
	inline Rational operator + (const Rational & r)const{
		return Rational(*this) += r;
	}
	inline Rational operator - (const Rational & r)const{
		return Rational(*this) -= r;
	}
	inline Rational operator * (const Rational & r)const{
		return Rational(*this) *= r;
	}
	inline Rational operator / (const Rational & r)const{
		return Rational(*this) /= r;
	}
	inline Rational operator - (void)const{
		return Rational(-this->p, this->q);
	}
	inline bool operator < (const Rational & r)const{
		return (*this - r).sgn() == -1;
	}
	inline bool operator <= (const Rational & r)const{
		return (*this - r).sgn() < 1;
	}
	inline bool operator == (const Rational & r)const{
		return (*this - r).sgn() == 0;
	}
	inline bool operator > (const Rational & r)const{
		return !(*this <= r);
	}
	inline bool operator >= (const Rational & r)const{
		return !(*this < r);
	}
	inline bool operator != (const Rational & r)const{
		return !(*this == r);
	}
};
