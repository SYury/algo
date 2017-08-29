#include<bits/stdc++.h>

using namespace std;
typedef long long lint;
typedef long double ldb;
typedef unsigned long long uli;

#define X first
#define Y second
#define F(i, l, r) for(int i = l; i != r; i++)
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

const int block_size = 100000;
const int n = 100000000;
const int sqrtn = 10001;
bool block[block_size];
int sieve[sqrtn];
int p[sqrtn];
int last[sqrtn];

void run(){
	//Outputs every 1st mod 100 prime
	int ptr = 0;
	F(i, 2, sqrtn + 1){
		if(sieve[i])cont;
		last[ptr] = i + i;
		p[ptr++] = i;
		if(i * 1ll * i > sqrtn)cont;
		for(int j = i * i; j <= sqrtn; j += i)sieve[j] = true;
	}
	int id = 1;
	int maxblock = n/block_size;
	int l = 0;
	for(int block_id = 0; block_id <= maxblock; block_id++){
		memset(block, 0, sizeof(block));
		F(i, 0, ptr){				
			int j = last[i] - l;
			while(j < block_size){block[j] = true; j += p[i];}
			last[i] = j + l;
		}
		if(block_id == 0)block[0] = block[1] = true;
		for(int i = 0; i < block_size && l + i <= n; i++)if(!block[i]){if(id == 1)printf("%d\n", i + l); id++; if(id >= 100)id -= 100;}
		l += block_size;
	}
};

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
//	clock_t start = clock();
	memset(sieve, 0, sizeof(sieve));
	run();
//	printf("Elapsed time: %.5lf\n", (clock() - start)/(double)CLOCKS_PER_SEC);
	ret 0;
}
