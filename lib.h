#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

class Node {
public:
  vector<int> adj;
  Node() {
    adj.resize(0);
  }
};

class Digraph {
public:
  vector<Node*> nodes;
  
  Digraph(int v) { 
    nodes.resize(v);
    for (int i=0; i<v; i++) {
      nodes[i] = new Node();
    }
  }

  void addEdge(int v, int w) {
    for (auto it : nodes[v]->adj) {
      if (it == w) return;
    }
    nodes[v]->adj.push_back(w);
  }

};

class NFA {
private:
  string simplify(string regex) {
    // substitutes the + operator for the adequate * operator
    string simp = "";
    for (int i=0; i<(int)regex.length(); i++) {
      if (regex[i] == '+') {
        if (regex[i-1] == ')') {
          int j = i-1, cnt = 1;
          while (cnt > 0) {
            j--;
            if (regex[j] == ')') cnt++;
            else if (regex[j] == '(') cnt--;
          }
          simp += regex.substr(j, i-j);
          simp += '*';
        }
        else {
          simp += regex.substr(i-1, 1);
          simp += '*';
        }
      }
      else {
        simp += regex.substr(i, 1);
      }
    }
    return simp;
  }

public:
  string RE;
  int M;
  Digraph *G;

  NFA(string regex) {
    RE = simplify(regex);
    M = RE.length();
    G = new Digraph(M+1);

    stack<int> sta;
    for (int i=0; i<M; i++) {
      int prev = i;
      if (RE[i] == '(' || RE[i] == '|' ) {
        sta.push(i);
      }
      else if (RE[i] == ')') {
        int optop = sta.top(); sta.pop();
        if (optop == '|') {
          prev = sta.top(); sta.pop();
          G->addEdge(prev, optop+1);
          G->addEdge(optop, i);
        }
        else { // optop == '('
          prev = optop;
        }
      }
      if (i<M-1 && RE[i+1] == '*') {
        G->addEdge(prev, i+1);
        G->addEdge(i+1, prev);
      }
      if (i<M-1 && (RE[i] == '(' || RE[i] == '*' || RE[i] == ')')) {
        G->addEdge(i, i+1);
      }
    }
  }

  void dfs(int v, vector<bool> &marked) {
    marked[v] = true;
    for (auto it : G->nodes[v]->adj) {
      if (!marked[it]) {
        dfs(it, marked);
      }
    }
  }

  bool recognizes(string txt) {
    vector<bool> reachable(M, false);
    dfs(0, reachable);
    for (int i=0; i<(int)txt.length(); i++) {
      vector<bool> now(M, false);
      for (int j=0; j<M; j++) {
        if (reachable[j]) {
          if (RE[j] == txt[i] || RE[j] == '.') {
            now[j+1] = true;
          }
          else if (RE[j] == '[') {
            if (RE[j+1] == '^') {
              bool found = false;
              int k = j+2;
              while (RE[k] != ']') {
                if (RE[k] == txt[i]) {
                  found = true;
                  break;
                }
                k++;
              }
              if (!found) now[j+1] = true;
            }
            else if (RE[j+2] == '-') {
              if (RE[j+1] <= txt[i] && txt[i] <= RE[j+3]) {
                now[j+4] = true;
              }
            }
            else {
              int k = j+1;
              while (RE[k] != ']') {
                if (RE[k] == txt[i]) {
                  now[k+1] = true;
                }
                k++;
              }

            }
          }
        }
        else if (RE[j] == '\\' && RE[j+1] == txt[i]) {
          now[j+2] = true;
        }
      }
      vector<bool> marked(M, false);
      for (auto x : reachable) x = false;
      for (int j=0; j<M; j++) {
        if (now[j]) {
          for (auto x : marked) x = false; 
          dfs(j, marked);
          for (int k=0; k<M; k++) {
            if (marked[k] && (G->nodes[k]->adj.size() == 0 || G->nodes[k]->adj[0] != k-1)) {
              reachable[k] = true;
            }
          }
        }
      }
    }
    return reachable[M-1];
  }

};
