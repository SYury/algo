#include <bits/stdc++.h>
using namespace std;

const int MAXA = 1e7 + 7;
int least_prime[MAXA];
int prs[MAXA];
int prime_cnt = 0;

void sieve(){
	for(int i = 2; i < MAXA; i++){
		if(least_prime[i] == 0){
			least_prime[i] = i;
			prs[prime_cnt++] = i;
		}
		for(int j = 0; j < prime_cnt && prs[j] <= least_prime[i] && i * 1ll * prs[j] < MAXA; j++)
			least_prime[i * prs[j]] = prs[j];
	}
}

int main() {
	sieve();
	printf("%d\n", prime_cnt);
	return 0;
} 
