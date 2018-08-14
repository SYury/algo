//ADD EDMONDS (graphs/edmonds.cpp) HERE

const int MAXN = 101;
edmonds<MAXN> msc2_solver;
int N; // elements to cover

struct cset{
	bitset<MAXN> mask;
	int id;
	bool has(int i){return mask.test(i);}
};

vector<int> msc2(vector<cset> s, bitset<MAXN> covered, bool & can){
	auto old = covered;
	for(int i = 0; i < (int)s.size(); i++)covered |= s[i].mask;
	if(covered.count() != N){can = false; return vector<int>();}
	can = true;
	vector<pair<int, int> > e;
	for(auto c : s){
		int l = -1, r;
		for(int i = 0; i < N; i++)if(c.has(i)){if(l==-1)l = i; else r = i;}
		e.push_back(make_pair(l, r));
	}
	msc2_solver.init(N, e);
	msc2_solver.solve();
	vector<int> res;
	vector<pair<int, int> >  edges;
	for(int i = 0; i < N; i++){
		if(msc2_solver.match[i] > i){edges.push_back(make_pair(i, msc2_solver.match[i]));}
		else if(msc2_solver.match[i] == -1 && !(old.test(i))){
			if(msc2_solver.gr[i].empty()){can = false; return vector<int>();}
			int j = msc2_solver.gr[i][0];
			edges.push_back(make_pair(i, j));
		}
	}
	for(auto c : edges){
		for(int i = 0; i < (int)s.size(); i++){
			if(s[i].has(c.first) && s[i].has(c.second)){res.push_back(s[i].id); break;}
		}
	}
	return res;
}

vector<cset> del(bitset<MAXN> mask, vector<cset> s){
	vector<cset> res(0);
	for(int i = 0; i < (int)s.size(); i++){
		s[i].mask &= (~mask);
		if(s[i].mask.any())res.push_back(s[i]);
	}
	return res;
}

inline bitset<MAXN> cover(bitset<MAXN> mask, bitset<MAXN> add){
	return mask|add;
}

vector<int> msc(vector<cset> s, bitset<MAXN> covered, bool & can){
	if(s.size() == 0){if(covered.count() != N)can = false; else can = true; return vector<int>();}
	for(int i = 0; i < (int)s.size(); i++){
		for(int j = 0; j < (int)s.size(); j++){
			if(i == j)continue;
			if((s[i].mask|s[j].mask)==s[j].mask){
				s.erase(s.begin() + i);
				return msc(s, covered, can);
			}
		}
	}
	for(int i = 0; i < N; i++){
		if(covered.test(i))continue;
		int cnt = 0;
		int pos = -1;
		for(int j = 0; j < (int)s.size(); j++){
			if(s[j].has(i)){pos = j; cnt++;}
		}
		if(cnt == 1){
			auto nc = cover(covered, s[pos].mask);
			vector<int> tmp = msc(del(s[pos].mask, s), nc, can);
			tmp.push_back(s[pos].id);
			return tmp;
		}
	}
	int pos = -1, sz = 0;
	for(int i = 0; i < (int)s.size(); i++){
		int len = s[i].mask.count();
		if(len > sz){sz = len; pos = i;}
	}
	if(sz == 2)return msc2(s, covered, can);
	bool kek, bishkek;
	auto nc0 = cover(covered, s[pos].mask);
	vector<int> res = msc(del(s[pos].mask, s), nc0, kek);
	res.push_back(s[pos].id);
	s.erase(s.begin() + pos);
	vector<int> res1 = msc(s, covered, bishkek);
	if(!kek){can = bishkek; return res1;}
	if(!bishkek){can = kek; return res;}
	can = true;
	if(res.size() < res1.size())return res; else return res1;
}
