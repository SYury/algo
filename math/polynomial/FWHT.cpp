#include<bits/stdc++.h>

using namespace std;

template<typename T>
struct XorTransform{
	void transform(T* l, T* r){
		if(r - l == 1)return;
		T* mid = l + (r - l)/2;
		transform(l, mid);
		transform(mid, r);
		for(; mid != r; l++, mid++){
			T x = *l, y = *mid;
			*l = x + y;
			*mid = x - y;
		}
	}
	void inverse(T* l, T* r){
		transform(l, r);
		int n = r - l;
		for(; l != r; l++)*l = *l/n;
	}
};

template<typename T>
struct OrTransform{
	void transform(T* l, T* r){
		if(r - l == 1)return;
		T* mid = l + (r - l)/2;
		transform(l, mid);
		transform(mid, r);
		for(; mid != r; l++, mid++){
			*mid += *l;
		}
	}
	void inverse(T* l, T* r){
		if(r - l == 1)return;
		T* mid = l + (r - l)/2;
		inverse(l, mid);
		inverse(mid, r);
		for(; mid != r; l++, mid++){
			*mid -= *l;
		}
	}
};

template<typename T>
struct AndTransform{
	void transform(T* l, T* r){
		if(r - l == 1)return;
		T* mid = l + (r - l)/2;
		transform(l, mid);
		transform(mid, r);
		for(; mid != r; l++, mid++){
			T x = *l, y = *mid;
			*l = y;
			*mid = x + y;
		}
	}
	void inverse(T* l, T* r){
		if(r - l == 1)return;
		T* mid = l + (r - l)/2;
		inverse(l, mid);
		inverse(mid, r);
		for(; mid != r; l++, mid++){
			T x = *l, y = *mid;
			*l = y - x;
			*mid = x;
		}
	}
};
