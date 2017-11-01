#include<bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

typedef long long int ll;

namespace LinkCut{
	struct node{
		node *p = NULL;
		node * pathparent = NULL;
		node *l = NULL, *r = NULL;
		int sz = 1;
		int value;
		int id;
		bool rev = false;
		ll sum;
		node(){}
		node(int _value, int _id):value(_value),id(_id){sum = value;}
		node(int _value):value(_value){sum = value;}
	};
	int sz(node * v){return (v) ? v->sz : 0;}
	ll sum(node * v){return (v) ? v->sum : 0;}
	void upd_node(node * v){if(v) v->sz = 1 + sz(v->l) + sz(v->r); if(v) v->sum = v->value + sum(v->l) + sum(v->r);}
	void push(node * v){
		if(v && v->rev){
			if(v->l)v->l->rev ^= 1;
			if(v->r)v->r->rev ^= 1;
			v->rev = 0;
			swap(v->l, v->r);
		}
	}
	void rotate(node * v){
		node * p = v->p;
		swap(p->pathparent, v->pathparent);
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
		upd_node(p);
		upd_node(v);
	}
	inline void zig(node * v){
		rotate(v);
	}
	inline void zig_zig(node * v){
		rotate(v->p);
		rotate(v);
	}
	inline void zig_zag(node * v){
		rotate(v);
		rotate(v);
	}
	void splay(node * v){
		push(v);
		while(v->p){
			if(v->p->p == NULL){push(v->p); push(v); zig(v);}
			else{
				push(v->p->p);
				push(v->p);
				push(v);
			   	if((v->p->l == v && v->p->p->l == v->p)||(v->p->r == v && v->p->p->r == v->p))zig_zig(v);
				else zig_zag(v);
			}
		}
	}
	void expose(node * u){
		splay(u);
		if(u->r){
			u->r->p = NULL;
			u->r->pathparent = u;
			u->r = NULL;
			upd_node(u);
		}
		node * v = u;
		while(v->pathparent){
			node * p = v->pathparent;
			splay(p);
			if(p->r){p->r->pathparent = p; p->r->p = NULL;}
			p->r = v;
			v->p = p;
			v->pathparent = NULL;
			upd_node(p);
			v = p;
		}
		splay(u);
	}
	node * find_root(node * v){
		expose(v);
		node * u = v;
		while(u->l){
			u = u->l;
			push(u);
		}
		return u;
	}
	void make_root(node * v){
		expose(v);
		v->rev ^= 1;
	}
	bool link(node * v, node * u){
		if(find_root(v) == find_root(u))return false;
		make_root(u);
		expose(v);
		u->pathparent = v;
		return true;
	}
	void cut(node * v){
		expose(v);
		if(v->l){v->l->pathparent = NULL; v->l->p = NULL;}
		v->l = NULL;
		v->pathparent = NULL;
		upd_node(v);
	}
	node * lca(node * v, node * u){
		expose(v);
		expose(u);
		splay(v);
		if(!v->pathparent){
			node * prv, * kek;
			kek = u;
			while(kek != v){
				prv = kek;
				kek = kek->p;
			}
			if(prv == v->l)return u;
			else return v;
		}
		else return v->pathparent;
	}
	ll get_sum(node * v){
		expose(v);
		return sum(v->l) + v->value;
	}
	ll query(node * v, node * u){
		node * kek = lca(v, u);
		return get_sum(v) + get_sum(u) - 2 * get_sum(kek) + kek->value;
	}
	void change(node * v, int nval){
		expose(v);
		splay(v);
		v->value = nval;
		upd_node(v);
	}
};

const int MAXN = 1e5 + 5;

LinkCut::node * what[MAXN];

int n, q;

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cin >> n;
	for(int i = 0; i < n; i++){int x; cin >> x; what[i] = new LinkCut::node(x, i);}
	cin >> q;
	for(int i = 0; i < q; i++){
		string tp;
		cin >> tp;
		if(tp == "bridge"){
			int v, u;
			cin >> v >> u;
			v--; u--;
			if(LinkCut::link(what[v], what[u])){
				cout << "yes" << '\n';
			}
			else cout << "no" << '\n';
		}
		else if(tp == "penguins"){
			int v, cnt;
			cin >> v >> cnt;
			v--;
			LinkCut::change(what[v], cnt);
		}
		else {
			int v, u;
			cin >> v >> u;
			v--; u--;
			if(LinkCut::find_root(what[v]) != LinkCut::find_root(what[u]))cout << "impossible" << '\n';
			else cout << LinkCut::query(what[v], what[u]) << '\n';
		}
	}
	return 0;
}
