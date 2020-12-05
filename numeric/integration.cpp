template<typename F>
F integrate(F (*f)(F), F a, F b, int nodes){
    F d = (b - a)/(nodes + 1);
    F ans = 0;
    for(int i = 0; i < nodes + 1; i++){
        F L = a, R = a + d;
        ans += d*(f(L) + f(R) + 4*f(0.5 * (L + R)))/6;
        a = R;
    }
    return ans;
}
