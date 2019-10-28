
//degree >= 2, P_{degree} != 0
//probably doesn't work with two equal roots
template<typename Float>
vector<complex<Float> > find_roots(vector<complex<Float> > poly,
        int ITERS = 500 /*number of Newton's method iterations for each candidate point*/,
        Float eps = (Float)1e-9 /*used to compare two roots for equality (|z_1 - z_2| < eps)*/){
    int d = poly.size() - 1;
    Float scale = 0.0;
    for(auto c : poly)
        scale += abs(c);
    scale = max(scale, (Float)1.0);
    //all the roots lie in |z| <= scale
    int circles = max(1, int(ceil(0.26632 * log(d)) + 0.1));
    int pts = max(8, int(ceil(8.32547 * d * log(d)) + 0.1));
    vector<complex<Float> > start;
    vector<complex<Float> > R1(circles), R2(pts);
    Float C = 1 + sqrt(2.0);
    for(int s = 0; s < circles; s++){
        R1[s] = C * pow((d - 1)/Float(d), (2 * s - 1)/Float(4 * circles));
    }
    Float PI = acos(-1.0);
    auto I = complex<Float>((Float)0, (Float)1);
    for(int v = 0; v < pts; v++){
        R2[v] = 2 * PI * v * I/Float(pts);
    }
    for(int s = 0; s < circles; s++){
        for(int v = 0; v < pts; v++){
            start.push_back(R1[s] * exp(R2[v]) * scale);
        }
    }
    vector<complex<Float> > deriv(d);
    for(int i = 1; i <= d; i++){
        deriv[i - 1] = poly[i] * Float(i);
    }
    auto eval = [](complex<Float> z, const vector<complex<Float> > & p){
        complex<Float> res = 0;
        complex<Float> pw = 1;
        for(int i = 0; i < (int)p.size(); i++){
            res += p[i] * pw;
            pw *= z;
        }
        return res;
    };
    auto go = [&poly, &deriv, &ITERS, &eval](complex<Float> z){
        for(int i = 0; i < ITERS; i++){
            z -= eval(z, poly)/eval(z, deriv);
        }
        return z;
    };
    vector<complex<Float> > roots;
    for(auto z : start){
        auto cand = go(z);
        bool ok = 1;
        for(auto z1 : roots){
            auto diff = cand - z1;
            if(abs(diff) < eps){ok = 0; break;}
        }
        if(ok)
            roots.push_back(cand);
    }
    return roots;
}
