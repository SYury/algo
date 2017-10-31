#include<bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

namespace SplayTree{
	template<typename T>
	struct node{
		node *p = NULL;
		node *l = NULL, *r = NULL;
		T key;
		size_t sz = 1;
		node(){}
		node(T _key):key(_key){}
	};
	template<typename T>
	size_t sz(node<T> * v){return (v) ? v->sz : 0;}
	template<typename T>
	void upd_sz(node<T> * v){if(v) v->sz = 1 + sz(v->l) + sz(v->r);}
	template<typename T>
	void rotate(node<T> * v){
		node<T> * p = v->p;
		if(p->l == v){
			p->l = v->r;
			if(v->r)v->r->p = p;
			if(p->p){if(p->p->l == p)p->p->l = v; else p->p->r = v;}
			v->p = p->p;
			p->p = v;
			v->r = p;
		}
		else{
			p->r = v->l;
			if(v->l)v->l->p = p;
			if(p->p){if(p->p->l == p)p->p->l = v; else p->p->r = v;}
			v->p = p->p;
			p->p = v;
			v->l = p;
		}
		upd_sz(p);
		upd_sz(v);
	}
	template<typename T>
	inline void zig(node<T> * v){
		rotate(v);
	}
	template<typename T>
	inline void zig_zig(node<T> * v){
		rotate(v->p);
		rotate(v);
	}
	template<typename T>
	inline void zig_zag(node<T> * v){
		rotate(v);
		rotate(v);
	}
	template<typename T>
	void splay(node<T> * v){
		while(v->p){
			if(v->p->p == NULL)zig(v);
			else if((v->p->l == v && v->p->p->l == v->p)||(v->p->r == v && v->p->p->r == v->p))zig_zig(v);
			else zig_zag(v);
		}
	}
	template<typename T>
	node<T> * _find(node<T> * v, T key){
		if(!v)return NULL;
		if(v->key == key){splay(v); return v;}
		if(v->key < key)return _find(v->r, key);
		else return _find(v->l, key);
	}
	template<typename T>
	node<T> * _upper_bound(node<T> * v, T x){
		if(!v)return NULL;
		if(v->key <= x){
			return _upper_bound(v->r, x);
		}
		else{
			node<T> * u = _upper_bound(v->l, x);
			if(!u)return v;
			else return u;
		}
	}
	template<typename T>
	node<T> * _lower_bound(node<T> * v, T x){
		if(!v)return NULL;
		if(v->key == x)return v;
		if(v->key < x){
			return _lower_bound(v->r, x);
		}
		else{
			node<T> * u = _lower_bound(v->l, x);
			if(!u)return v;
			else return u;
		}
	}
	template<typename T>
	node<T> * merge(node<T> * t1, node<T> * t2){
		if(!t1)return t2;
		if(!t2)return t1;
		while(t1->r)t1 = t1->r;
		splay(t1);
		t2->p = t1;
		t1->r = t2;
		upd_sz(t1);
		return t1;
	}
	template<typename T>
	void split(node<T> * v, node<T> * & t1, node<T> * & t2, T x){
		node<T> * old = v;
		v = _upper_bound(v, x);
		if(!v){t1 = old; t2 = NULL; return;}
		splay(v);
		t2 = v; t1 = v->l;
		t2->l = NULL; 
		if(t1)t1->p = NULL;
		upd_sz(t2);
	}
	template<typename T>
	node<T> * _find_by_order(node<T> * v, size_t k){
		if(!v)return NULL;
		if(k < sz(v->l))return _find_by_order(v->l, k);
		if(k == sz(v->l)){
			splay(v);
			return v;
		}
		return _find_by_order(v->r, k - sz(v->l) - 1);
	}
	template<typename T>
	class SplayTree{
		node<T> * root = NULL;
		size_t csz = 0;
		public:
		size_t size();
		bool empty();
		node<T> * find(T key);
		node<T> * lower_bound(T key);
		node<T> * upper_bound(T key);
		node<T> * find_by_order(size_t key);
		node<T> * min();
		node<T> * max();
		bool contains(T key);
		void insert(T key);
		void erase(T key);
		void erase(node<T> * v);
	};
	template<typename T>
	size_t SplayTree<T>::size(){return csz;}
	template<typename T>
	bool SplayTree<T>::empty(){return size() == 0;}
	template<typename T>
	void SplayTree<T>::insert(T key){
		node<T> * v = new node<T>(key);
		node<T> *l, *r;
		split(root, l, r, key);
		root = merge(l, v);
		root = merge(root, r);
		csz++;
	}
	template<typename T>
	void SplayTree<T>::erase(T key){
		node<T> * v = _find(root, key);
		if(!v)return;
		if(v->l)v->l->p = NULL;
		if(v->r)v->r->p = NULL;
		root = merge(v->l, v->r);
		csz--;
	}
	template<typename T>
	void SplayTree<T>::erase(node<T> * v){
		if(!v)return;
		splay(v);
		if(v->l)v->l->p = NULL;
		if(v->r)v->r->p = NULL;
		root = merge(v->l, v->r);
		csz--;
	}
	template<typename T>
	node<T> * SplayTree<T>::lower_bound(T key){
		node<T> * res = _lower_bound(root, key);
		if(res){splay(res); root = res;}
		return res;
	}
	template<typename T>
	node<T> * SplayTree<T>::upper_bound(T key){
		node<T> * res = _upper_bound(root, key);
		if(res){splay(res); root = res;}
		return res;
	}
	template<typename T>
	node<T> * SplayTree<T>::find(T key){
		node<T> * res = _find(root, key);
		if(res)root = res;
		return res;
	}
	template<typename T>
	bool SplayTree<T>::contains(T key){
		return find(key) != NULL;
	}
	template<typename T>
	node<T> * SplayTree<T>::find_by_order(size_t k){
		if(k >= size())return NULL;
		node<T> * v = _find_by_order(root, k);
		if(v)root = v;
		return v;
	}
	template<typename T>
	node<T> * SplayTree<T>::min(){
		node<T> * v = root;
		if(!v)return NULL;
		while(v->l)v = v->l;
		splay(v);
		root = v;
		return v;
	}
	template<typename T>
	node<T> * SplayTree<T>::max(){
		node<T> * v = root;
		if(!v)return NULL;
		while(v->r)v = v->r;
		splay(v);
		root = v;
		return v;
	}
};

SplayTree::SplayTree<int> s;

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	int n = 0;
	while(true){
		int x;
		scanf("%d", &x);
		if(x == 0)break;
		if(!s.find(x)){
			n++;
			s.insert(x);
		}
	}
	printf("%d", s.find_by_order(n - 2)->key);
	return 0;
}
