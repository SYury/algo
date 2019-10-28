//indexes
//0: constant
//1..N: non-basic variables
//N+1..B+N+1: basic variables
template<typename F>
class CanonicalSolver{
public:
    static F* solve_feasible(int B, int N, int * lhs,
            F ** rhs, F * func, F eps){
        F * values = new F[B + N + 1];
        memset(values, 0, sizeof(F) * (B + N + 1));
        for(int i = 0; i < B; i++)
            values[lhs[i]] = rhs[i][0];
        values[0] = 1;
        bool * basis = new bool[B + N + 1];
        memset(basis, 0, sizeof(bool) * (B + N + 1));
        while(1){
            int pos = -1;
            for(int i = 0; i < B; i++)
                basis[lhs[i]] = 1;
            for(int i = 1; i < B + N + 1; i++){
                if(basis[i] || func[i] < eps)
                    continue;
                if(pos == -1 || func[i] > func[pos])
                    pos = i;
            }
            for(int i = 0; i < B; i++)
                basis[lhs[i]] = 0;
            if(pos == -1)break;
            F bnd = 0;
            bool was = 0;
            int what = 0;
            for(int i = 0; i < B; i++){
                if(rhs[i][pos] > -eps)
                    continue;
                F curr = values[lhs[i]];
                curr /= -rhs[i][pos];
                if(!was || bnd > curr){
                    was = 1;
                    what = i;
                    bnd = curr;
                }
            }
            if(!was)
                return nullptr;
            for(int i = 0; i < B; i++)
                values[lhs[i]] += bnd * rhs[i][pos];
            int old = lhs[what];
            lhs[what] = pos;
            values[pos] += bnd;
            F oldval = 1/rhs[what][pos];
            for(int i = 0; i < 1 + B + N; i++)
                rhs[what][i] *= -oldval;
            rhs[what][old] = oldval;
            rhs[what][pos] = 0;
            for(int i = 0; i < B; i++){
                if(i == what)
                    continue;
                F coeff = rhs[i][pos];
                rhs[i][pos] = 0;
                for(int j = 0; j < 1 + B + N; j++)
                    rhs[i][j] += rhs[what][j] * coeff;
            }
            F coeff = func[pos];
            func[pos] = 0;
            for(int j = 0; j < 1 + B + N; j++)
                func[j] += rhs[what][j] * coeff;
        }
        delete[] basis;
        return values;
    }
    //0: solution exists
    //1: unbounded
    //-1: unfeasible
    static pair<F*, int> solve(int B, int N, int * lhs,
            F ** rhs, F * func, F eps){
        bool fea = 1;
        for(int i = 0; i < B; i++)
            if(rhs[i][0] < -eps){fea = 0; break;}
        if(fea){
            auto res = solve_feasible(B, N, lhs, rhs,
                    func, eps);
            return res == nullptr ? make_pair(res, 1) :
                make_pair(res, 0);
        }
        int pos = 0;
        for(int i = 1; i < B; i++)
            if(rhs[i][0] < rhs[pos][0])
                pos = i;
        int * new_lhs = new int[B];
        memcpy(new_lhs, lhs, B * sizeof(int));
        F ** new_rhs = (F**)malloc(B * sizeof(F*));
        for(int i = 0; i < B; i++){
            new_rhs[i] = (F*)malloc((2 + B + N) *
                    sizeof(F));
            memcpy(new_rhs[i], rhs[i], (1 + B + N) *
                    sizeof(F));
            new_rhs[i][1 + B + N] = 1;
        }
        F * new_func = new F[2 + N + B];
        memset(new_func, 0, sizeof(F) * (2 + N + B));
        new_rhs[pos][1 + N + B] = 0;
        for(int j = 0; j < 2 + N + B; j++)
            new_rhs[pos][j] = -new_rhs[pos][j];
        new_rhs[pos][lhs[pos]] = 1;
        new_lhs[pos] = 1 + N + B;
        for(int i = 0; i < B; i++){
            if(pos == i)
                continue;
            new_rhs[i][1 + N + B] = 0;
            for(int j = 0; j < 1 + N + B; j++)
                new_rhs[i][j] += new_rhs[pos][j];
        }
        for(int i = 0; i < 1 + N + B; i++)
            new_func[i] = -new_rhs[pos][i];
        auto res_lambda = solve_feasible(B, N + 1, new_lhs,
                new_rhs, new_func, eps);
        if(res_lambda == nullptr)
            return make_pair(nullptr, -1);
        F cres = 0;
        for(int i = 0; i < 2 + N + B; i++)
            cres += res_lambda[i] * new_func[i];
        if(abs(cres) > eps)
            return make_pair(nullptr, -1);
        int bpos = -1;
        for(int i = 0; i < B; i++)
            if(new_lhs[i] == 1 + N + B){
                bpos = i;
                break;
            }
        if(bpos == -1){
            memcpy(lhs, new_lhs, B * sizeof(int));
            for(int i = 0; i < B; i++)
                memcpy(rhs[i], new_rhs[i], (1 + B + N) *
                        sizeof(F));
            memcpy(new_func, func, (1 + B + N) * sizeof(F));
            for(int i = 0; i < B; i++){
                F coeff = func[new_lhs[i]];
                    new_func[new_lhs[i]] = 0;
                for(int j = 0; j < 1 + B + N; j++)
                    new_func[j] += coeff * new_rhs[i][j];
            }
            memcpy(func, new_func, (1 + B + N) * sizeof(F));
            auto res = solve_feasible(B, N, lhs, rhs,
                    func, eps);
            return res == nullptr ? make_pair(res, 1) :
                make_pair(res, 0);
        }
        int with_what = -1;
        for(int i = 1; i < 1 + N + B; i++){
            if(abs(new_rhs[bpos][i]) > eps){
                with_what = i;
                break;
            }
        }
        F coeff = -new_rhs[bpos][with_what];
        new_rhs[bpos][with_what] = 0;
        new_rhs[bpos][new_lhs[bpos]] = -1;
        new_lhs[bpos] = with_what;
        for(int j = 0; j < 2 + N + B; j++)
            new_rhs[bpos][j] /= coeff;
        for(int i = 0; i < B; i++){
            if(i == bpos)
                continue;
            F coeff = new_rhs[i][with_what];
            for(int j = 0; j < 2 + N + B; j++)
                new_rhs[i][j] += coeff * new_rhs[bpos][j];
        }
        memcpy(lhs, new_lhs, B * sizeof(int));
        for(int i = 0; i < B; i++)
            memcpy(rhs[i], new_rhs[i], (1 + B + N) *
                    sizeof(F));
        memcpy(new_func, func, (1 + B + N) * sizeof(F));
        for(int i = 0; i < B; i++){
            F coeff = func[new_lhs[i]];
            new_func[new_lhs[i]] = 0;
            for(int j = 0; j < 1 + B + N; j++)
                new_func[j] += coeff * new_rhs[i][j];
        }
        memcpy(func, new_func, (1 + B + N) * sizeof(F));
        auto res = solve_feasible(B, N, lhs, rhs, func, eps);
        return res == nullptr ? make_pair(res, 1) :
            make_pair(res, 0);
    }
};
