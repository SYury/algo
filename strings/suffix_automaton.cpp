#include<string>
#include<vector>
#include<map>
#include<utility>

using namespace std;

struct state{
  int len,link;
  map<char,int>next;
};

vector<state>st;
int last=0;

void sa_init(){
  st.clear();
  st.resize(1);
  st[0].len=0;
  st[0].link=-1;
}

void sa_extend(char c){
  state current;
  st.push_back(current);
  int curr=st.size()-1;
  st[curr].len=st[last].len+1;
  int p;
  for (p=last;p!=-1 && !st[p].next.count(c);p=st[p].link){
    st[p].next[c]=curr;
  }
  if(p==-1){st[curr].link=0;}
  else{
    int q=st[p].next[c];
    if(st[p].len+1==st[q].len){st[curr].link=q;}
    else{
      st.push_back(current);
      st[curr+1]=st[q];
      st[curr+1].len=st[p].len+1;
      for(;p!=-1 && st[p].next[c]==q;p=st[p].link){
	st[p].next[c]=curr+1;
      }
      st[curr+1].link=curr+1;st[q].link=curr+1;
    }
  }
  last=curr;
}

void sa_build(string s){
  sa_init();
  for (size_t i=0;i<s.length();i++){
    sa_extend(s[i]);
  }
}