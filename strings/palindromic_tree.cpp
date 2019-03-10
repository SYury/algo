template<size_t N>
struct PTree{
	struct node{
		map<char, int> nxt;
		int link;
		int len;
	};

	node t[N + 3];
	int sz;
	int last;

	node& operator [] (const int & pos){
		return t[pos];
	}

	const node& operator [] (const int & pos)const{
		return t[pos];
	}

	void init(){
		sz = 2;
		t[0].link = 1;
		t[0].len = 0;
		t[1].len = t[1].link = -1;
		last = 1;
	}

	void extend(const string & s, int pos){
		int v = last;
		while(1){
			if(s[pos] != s[pos - 1 - t[v].len])
				v = t[v].link;
			else break;
		}
		if(t[v].nxt.count(s[pos])){
			v = t[v].nxt[s[pos]];
		}
		else{
			int u = sz++;
			t[v].nxt[s[pos]] = u;
			t[u].len = t[v].len + 2;
			while(v != -1){
				v = t[v].link;
				if(s[pos] == s[pos - 1 - t[v].len])
					break;
			}
			if(v == -1){
				t[u].link = 0;
			}
			else t[u].link = t[v].nxt[s[pos]];
			v = u;
		}
		last = v;
	}

	void build(const string & s){
		init();
		for(int i = 0; i < (int)s.length(); i++)
			extend(s, i);
	}
};
