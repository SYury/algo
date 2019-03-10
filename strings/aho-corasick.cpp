const int ALPHA = 26;
const char BASE = 'a';
struct AhoCorasick{
	struct node{
		node* nxt[ALPHA];
		node* link = nullptr;
		bool is_terminal = false;
		node(){fill(nxt, nxt + ALPHA, nullptr);}
		node * go(char c){return nxt[c - BASE];}
	};
	node * root = new node;
	void add_string(const string & s){
		node * v = root;
		for(char c : s){
			if(v->nxt[c - BASE] == nullptr)
				v->nxt[c - BASE] = new node;
			v = v->nxt[c - BASE];
		}
		v->is_terminal = true;
	}
	void build_automaton(){
		queue<node*> q;
		q.push(root);
		root->link = root;
		while(!q.empty()){
			node * v = q.front();
			q.pop();
			node * link = v->link;
			if(!v->is_terminal)v->is_terminal = v->link->is_terminal;
			for(int c = 0; c < ALPHA; c++){
				if(v->nxt[c] != nullptr){
					v->nxt[c]->link = (v == root) ? root : link->nxt[c];
					q.push(v->nxt[c]);
				}
				else{
					v->nxt[c] = link->nxt[c];
					if(v->nxt[c] == nullptr)
						v->nxt[c] = root;
				}
			}
		}
	}
};
