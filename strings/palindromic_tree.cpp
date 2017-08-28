#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define Df(i, l, r) for(auto i = l; i != r; i--)
#define pb push_back
#define rs resize
#define mk make_pair
#define asg assign
#define all(x) x.begin(),x.end()
#define ret return
#define cont continue
#define brk break
#define ins insert
#define era erase
#define y1 adjf


string s;
vector<int> ep;

struct node{
	node * link = NULL, * pr = NULL;
	int len;
	int l, r;
	int dp;
	map<char, node*> nxt;
	bool can(int pos){
		if(len > 0){
			if(pos - len - 1 < 0){ret false;}
			ret s[pos - len - 1] == s[pos];
		}
		if(len == 0){
			if(r == -1){ret false;}
			ret s[r] == s[pos];
		}
		ret true;
	}
};

node * root[2];
node * last;
int pcnt = 0;

void p_init(){
	root[0] = new node; root[1] = new node;
	root[0]->len = -1;
	root[1]->link = root[0];
	root[1]->len = 0;
	root[1]->pr = root[0];
	root[1]->r = -1;
	root[0]->dp = root[1]->dp = 0;
	root[0]->nxt.clear();
	root[1]->nxt.clear();
	last = root[1];
}

void p_add(int pos){
	char c = s[pos];
	node * p = last;
	while(!p->can(pos)){p = p->link;}
	if(p->nxt.count(c)){
		p = p->nxt[c];
		last = p;
		p->r = pos;
		p->l = pos - p->len + 1;
		root[1]->r = pos;
		ep[pos] = p->dp;
		pcnt += p->dp;
		ret;
	}
	node * v = new node;
//	pcnt++;
	v->len = p->len + 2;
	v->pr = p;
	v->r = pos;
	v->l = pos - v->len + 1;
	p->nxt[c] = v;
	if(p == root[0]){v->link = root[1];}
	else{
		p = p->link;
		while(!p->can(pos))
			p = p->link;
		p = p->nxt[c];
		v->link = p;
	}
	v->dp = v->link->dp + 1;
	ep[pos] = v->dp;
	pcnt += v->dp;
	root[1]->r = pos;
	last = v;
	//printf("After adding %d pcnt = %d\n", pos, pcnt);
}

void p_build(string & tmp){
	s = tmp;
	p_init();
	ep.rs(s.length());
	F(i, 0, (int)s.length())
		p_add(i);
}
 
