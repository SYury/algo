#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(auto i = l; i != r; i++)
#define Df(i, l, r) for(auto i = l; i != r; i--)
#define uF(i, l, r) for(i = l; i != r; i++)
#define uDf(i, l, r) for(i = l; i != r; i--)
#define I(a, x) for(auto a : x)
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
#define fi0(x) memset(x, 0, sizeof(x))
#define acpy(y, x) memcpy(y, x, sizeof(y))
#define y1 adjf
#define tm fjfjfk 

struct minstack{
	stack<pair<int, int> > stk;
	int get(){
		if(stk.empty())ret INT_MAX;
		else ret stk.top().Y;
	}
	void push(int x){
		if(stk.empty())stk.push(mk(x, x));
		int prv = get();
		stk.push(mk(x, min(x, prv)));
	}
	void pop(){
		stk.pop();
	}
	int top(){
		if(stk.empty())ret INT_MAX; else ret stk.top().X;
	}
	bool empty(){ret stk.empty();}
};

struct minqueue{
	minstack s1, s2;
	int get(){
		ret min(s1.get(), s2.get());
	}
	void push(int x){
		s1.push(x);
	}
	void pop(){
		if(!s2.empty()){s2.pop(); ret;}
		if(s1.empty())ret;
		while(!s1.empty()){
			s2.push(s1.top());
			s1.pop();
		}
		s2.pop();
	}
	bool empty(){ret s1.empty() && s2.empty();}
};
