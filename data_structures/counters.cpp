template<class T>
class EraseCounter{
	void dec(int pos){
		while(pos < (int)t.size()){
			++t[pos];
			pos += pos&-pos;
		}
	}
	int get(int pos){
		int res = 0;
		while(pos){
			res += t[pos];
			pos -= pos&-pos;
		}
		ret res;
	}
public:
	vector<int> t;
	vector<T> a;
	EraseCounter(){}
	EraseCounter(const vector<T> & kek){
		a = vector<T>(kek);
		t.resize(a.size() + 1);
		fill(t.begin(), t.end(), 0);
	}
	void erase(const T& x){
		auto it = lower_bound(a.begin(), a.end(), x);
		int i = it - a.begin();
		dec(i + 1);
	}
	void erase_all(const T& x){
		typename vector<T>::iterator l, r;
		tie(l, r) = equal_range(a.begin(), a.end(), x);
		int i = l - a.begin(), j = r - a.begin();
		for(; i < j; i++)
			dec(i + 1);
	}
	int less(const T& x){
		int i = lower_bound(a.begin(), a.end(), x) - a.begin();
		return i - get(i);
	}
	int less_equal(const T& x){
		int i = upper_bound(a.begin(), a.end(), x) - a.begin();
		return i - get(i);
	}
	int greater(const T& x){
		return (int)a.size() - get((int)a.size()) - less_equal(x);
	}
	int greater_equal(const T& x){
		return (int)a.size() - get((int)a.size()) - less(x);
	}
	int equal(const T& x){
		return less_equal(x) - less(x);
	}
	bool count(const T& x){
		return equal(x) != 0;
	}
};

template<class T>
class InsertCounter{
public:
    vector<T> a;
    void insert(const T& x) {
		int pk = a.size()&-a.size();
        a.push_back(x);
        int k = a.size()&-a.size();
        while(pk && pk < k){
			inplace_merge(a.end() - pk * 2, a.end() - pk, a.end());
			pk *= 2;
		}
    }
    bool count(const T& x) const {
        int p = a.size();
        while (p > 0) {
            int r = p&-p;
            p -= r;
            if (binary_search(a.begin() + p, a.begin() + p + r, x)) return true;
        }
        return false;
    }
	int less(const T& x){
		int p = a.size();
		int res = 0;
		while(p > 0){
			int r = p&-p;
			p -= r;
			res += lower_bound(a.begin() + p, a.begin() + p + r, x) - (a.begin() + p);
		}
		return res;
	}
	int less_equal(const T& x){
		int p = a.size();
		int res = 0;
		while(p > 0){
			int r = p&-p;
			p -= r;
			res += upper_bound(a.begin() + p, a.begin() + p + r, x) - (a.begin() + p);
		}
		return res;
	}
	int greater(const T& x){
		return (int)a.size() - less_equal(x);
	}
	int greater_equal(const T& x){
		return (int)a.size() - less(x);
	}
	int equal(const T& x){
		return (int)a.size() - greater(x) - less(x);
	}
};
