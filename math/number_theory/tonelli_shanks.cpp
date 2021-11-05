template<typename Int>
Int modpow(Int a, Int pw, Int mod){
    Int result = 1;
    while(pw > 0){
        if(pw%2 == 1){
            result = result * a % mod;
        }
        a = a * a % mod;
        pw /= 2;
    }
    return result;
}

template<typename Int>
bool is_square_residue_mod_prime(Int a, Int mod){
    if(mod == 2 || a == 0)
        return true;
    Int z = modpow(a, (mod - 1)/2, mod);
    return z == 1;
}

template<typename Int>
Int _tonelli_shanks_known_generator(Int a, Int mod, Int z){
    int64_t s = 0;
    Int q = mod - 1;
    while(q%2 == 0){
        q /= 2;
        ++s;
    }
    z = modpow(z, q, mod);
    Int x = modpow(a, (q - 1)/2, mod);
    Int b = x * a % mod * x % mod;
    x = x * a % mod;
    Int y = z;
    while(1){
        if(b == 1){
            return x;
        }
        int64_t m = 0;
        Int pwr = b;
        while(pwr != 1){
            pwr = pwr * pwr % mod;
            ++m;
        }
        if(m == s)
            return 0; // not a quadratic residue
        int64_t t_power = s - m - 1;
        while(t_power > 0){
            y = y * y % mod;
            --t_power;
        }
        Int t = y;
        y = y * y % mod;
        s = m;
        x = x * t % mod;
        b = b * y % mod;
    }
}

template<typename Int, class RandomGenerator>
Int tonelli_shanks_random(Int a, Int mod, RandomGenerator rnd){
    if(mod == 2)
        return a;
    while(1){
        Int z = rnd();
        if(!is_square_residue_mod_prime(z, mod))
            return _tonelli_shanks_known_generator(a, mod, z);
    }
}

template<typename Int>
Int tonelli_shanks_deterministic(Int a, Int mod){
    if(mod == 2)
        return a;
    for(Int z = 1;; z++){
        if(!is_square_residue_mod_prime(z, mod))
            return _tonelli_shanks_known_generator(a, mod, z);
    }
}
