#include "iostream"
#include "string.h"
#include "vector"
#include "stack"


using namespace std;

class Graph {
    public:
        int V;
        vector<vector<int>> adj;
        string exp;

        Graph(int V) {
            this->V = V;
            adj.resize(V);
        }

        void addEdge(int v, int w) {
            adj[v].push_back(w);
        }

        void dfs(int v, vector<bool> & atingidos) {
            atingidos[v] = true;
            for (auto i : adj[v]) {
                if (!atingidos[i]) {
                    dfs(i, atingidos);
                }
            }
        }

        void print() {
            for (int i = 0; i < V; i++) {
                cout << i << ": ";
                for (int j = 0; j < adj[i].size(); j++) {
                    cout << adj[i][j] << " ";
                }
                cout << endl;
            }
        }
};
//cria o grafo a partir da expressão regular
Graph criaGrafo (string exp){
    Graph G(exp.size()+1);
    G.exp = exp;
    stack<int> pilha;
    for (int i = 0; i < exp.size(); i++){
        int ant = i;
        if (exp[i] == '(' || exp[i] == '|'){
            pilha.push(i);
        } else if (exp[i] == ')'){
            int optopo = pilha.top();
            pilha.pop();
            if (exp[optopo] == '|'){
                ant = pilha.top();
                pilha.pop();
                G.addEdge(ant, optopo+1);
                G.addEdge(optopo, i);
            } else {
                ant = optopo;
            }
        }
        if (i < exp.size() && exp[i+1] == '*'){
            G.addEdge(ant, i+1);
            G.addEdge(i+1, ant);
        } 
        if (exp[i] == '*' || exp[i] == '(' || exp[i] == ')'){
            G.addEdge(i, i+1);
        }
    }
    return G;
}
//reconhece a expressão regular a partir do grafo
bool reconhece (Graph G, string teste){
    vector<bool> atingidos(G.V); // G.V == exp.size()+1 == M+1
    for (int i = 0; i < G.V; i++) {
        atingidos[i] = false;
    }
    G.dfs(0, atingidos);
    

    for (int i = 0; i < teste.size(); i++){
        bool * atual = new bool[G.V];
        for (int j = 0; j < G.V; j++) {
            atual[j] = false;
        }
        for (int j = 0; j < G.V; j++) {
            if (atingidos[j]) {
                if (G.exp[j] == teste[i] || G.exp[j] == '.')
                    atual[j+1] = true;
                else if (G.exp[j] == '['){
                    if (G.exp[j+1] == '^'){
                        int flag = 1, k;
                        for (k = j+2; G.exp[k] != ']'; k++){
                            if (G.exp[k] == teste[i]){
                                flag = 0;
                            }
                        }
                        if (flag)
                            atual[k+1] = true;
                    } else if (G.exp[j+2] == '-'){
                        if (teste[i] >= G.exp[j+1] && teste[i] <= G.exp[j+3]){
                            atual[j+5] = true;
                        }
                    } else {
                        bool flag = false;
                        int k;
                        for (k = j+1; G.exp[k] != ']'; k++){
                            if (G.exp[k] == teste[i]){
                                flag = true;
                            }
                        }
                        atual[k+1] = flag;
                    }
                } else if (G.exp[j] == '\\' && G.exp[j+1] == teste[i]){
                    atual[j+2] = true;
                }
            }
        }
        vector<bool> marcado(G.V);
        for (int j = 0; j < G.V; j++) {
            atingidos[j] = false;
        }
        for (int j = 0; j < G.V; j++){
            if (atual[j]){
                for (int k = 0; k < G.V; k++){
                    marcado[k] = false;
                }
                G.dfs(j, marcado);
                for (int k = 0; k < G.V; k++){
                    if (marcado[k]){
                        atingidos[k] = true;
                    }
                }
            }
        }
    }
    return atingidos[G.V-1];
}

//recebe uma expressão regular contendo + e transforma em uma sem o +
string transforma (string exp){
    string nova = "";
    for (int i = 0; i < exp.size(); i++){
        if (exp[i] == '+'){
            if (exp[i-1] == ')'){
                int cont = 0;
                for (int j = nova.size()-2; j >= 0; j--){
                    if (nova[j] == ')'){
                        cont++;
                    } else if (nova[j] == '('){
                        if (cont > 0){
                            cont--;
                        } else {
                            int tam = nova.size();
                            for (int k = j; k < tam; k++){
                                nova += nova[k];
                            }
                            nova += '*';
                            break;
                        }
                    }
                }
            } else {
                nova += exp[i-1];
                nova += '*';
            }
        } else {
            nova += exp[i];
        }
    }
    return nova;
}

int main (){
    string exp;
    int n;
    cin >> exp >> n;
    exp = transforma(exp);
    Graph G = criaGrafo(exp);
    while(n--){
        string teste;
        cin >> teste;
        if (reconhece(G, teste)){
            cout << "S" << endl;
        } else {
            cout << "N" << endl;
        }
    }
    return 0;
}