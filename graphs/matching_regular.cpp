// Perfect matching in d-regular bipartite graph
// O(V log V) (expected)


struct RegularMatch{
    RegularMatch(const vector<vector<int> > & _gr, vector<int> _left_part): gr(_gr), left_part(_left_part){
        match = vector<int>(gr.size(), -1);
        left_vec_pos = vector<int>(gr.size(), -1);
        for(size_t i = 0; i < left_part.size(); i++){
            left_vec_pos[left_part[i]] = i;
        }
    }

    int sample(const vector<int> & vec){
        return vec[gen() % vec.size()];
    }

    int sample_without(const vector<int> & vec, int banned){
        int result;
        do{
            result = sample(vec);
        } while(result == banned);
        return result;
    }

    bool augment(int v, int b){
        path.push_back(v);
        if(v == 1 + (int)gr.size())
            return true;
        if(b < 0)
            return false;
        int u;
        if(v == (int)gr.size()){
            u = sample(left_part);
        }
        else if(match[v] == -1){
            if(left_vec_pos[v] != -1)
                u = sample(gr[v]);
            else
                u = (int)gr.size() + 1;
        }
        else{
            if(left_vec_pos[v] == -1)
                ++b;
            u = left_vec_pos[v] != -1 ? sample_without(gr[v], match[v]) : match[v];
        }
        return augment(u, b - 1);
    }

    void simplify_path(){
        int current_time = 0;
        vector<int> stk(path.size());
        vector<int> verts(path.size());
        int ptr = 0;
        for(int v: path){
            verts[ptr] = v;
            stk[ptr++] = ++current_time;
            if(used[v]){
                int prv = used[v];
                --ptr;
                while(stk[ptr - 1] > prv){
                    used[verts[ptr - 1]] = 0;
                    --ptr;
                }
            }
            else{
                used[v] = current_time;
            }
        }
        for(int v: path){
            used[v] = 0;
        }
        path = verts;
        path.resize(ptr);
    }

    vector<int> run(int seed = 0){
        gen = mt19937(seed);
        int j = 0;
        int L = (int)left_part.size();
        used = vector<int>(gr.size(), 0);
        while(j < L){
            int b = 6 + 2 * L/(L - j);
            do{
                path.clear();
            } while(!augment(gr.size(), b - 1));
            path.pop_back();
            path.erase(path.begin());
            simplify_path();
            for(size_t i = 0; i + 1 < path.size(); i += 2){
                match[path[i]] = path[i + 1];
                match[path[i + 1]] = path[i];
            }
            int v = path[0];
            int u = left_part.back();
            if(v != u){
                swap(left_part[left_vec_pos[v]], left_part[left_vec_pos[u]]);
                swap(left_vec_pos[v], left_vec_pos[u]);
            }
            left_part.pop_back();
            ++j;
        }
        return match;
    }

    vector<int> used;
    vector<int> match;
    const vector<vector<int> > & gr;
    vector<int> left_vec_pos;
    vector<int> path;
    vector<int> left_part;
    mt19937 gen;
};
