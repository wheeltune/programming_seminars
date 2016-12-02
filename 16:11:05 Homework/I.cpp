#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int ALPH = 2;
const char MINCH = '0';

struct Node {
    Node() : Node(-1, -1) {}

    Node(int parent, int pChar) : parent(parent), pChar(pChar) {
        for (int i = 0; i < ALPH; ++i) {
            next[i] = -1;
            edge[i] = -1;
        }
        link = -1;
        isTerm = false;
        isBad = -1;
    }

    int next[ALPH];
    int edge[ALPH];
    int link;
    int parent;
    int pChar;
    bool isTerm;

    int isBad;
};
                                    
class AhoCorasic {
private:
    vector<Node> nodes;
    vector<int> colors;

    int getEdge(int pos, int c) {
        if (nodes[pos].edge[c] == -1) {
            if (nodes[pos].next[c] == -1) {
                nodes[pos].edge[c] = (pos == 0) ? 0 : getEdge(getLink(pos), c);
            } else {
                nodes[pos].edge[c] = nodes[pos].next[c];
            }
        }
        return nodes[pos].edge[c];
    }    

    int getLink(int pos) {
        if (nodes[pos].link == -1) {
            if (pos == 0 || nodes[pos].parent == 0) {
                nodes[pos].link = 0;
            } else {
                nodes[pos].link = getEdge(getLink(nodes[pos].parent), nodes[pos].pChar);
            }
        }
        return nodes[pos].link;
    }

    int getBad(int pos) {
        if (nodes[pos].isBad == -1) {
            if (pos == 0) {
                nodes[pos].isBad = 0;
            } else {
                nodes[pos].isBad = getBad(getLink(pos));
            }
        }
        return nodes[pos].isBad;
    }

    bool dfs(int v) {
        if (getBad(v) == 1) {
            colors[v] = 2;
            return false;
        }
        colors[v] = 1;
        for (int c = 0; c < ALPH; ++c) {
            int u = getEdge(v, c);
            if (colors[u] == 1) {
                return true;
            } else if (colors[u] == 0) {
                if (dfs(u)) {
                    return true;
                }
            }
        }
        colors[v] = 2;
        return false;
    }

public:
    AhoCorasic() {
        nodes.push_back(Node(0, false));
    }

    void addWord(const string &s) {
        int pos = 0;
        for (int i = 0; i < s.length(); ++i) {
            int c = (int) (s[i] - MINCH);
            if (nodes[pos].next[c] == -1) {
                nodes.push_back(Node(pos, c));
                nodes[pos].next[c] = (int) nodes.size() - 1;
            }
            pos = nodes[pos].next[c];
        }
        nodes[pos].isTerm = true;
        nodes[pos].isBad = 1;
    }

    bool solve() {
        colors.assign(nodes.size(), 0);

        return dfs(0);
    }

    void print() {
        for (int i = 0; i < nodes.size(); ++i) {
            cout << i << "\n";
            for (int j = 0; j < ALPH; ++j) {
                if (nodes[i].next[j] != -1) {
                    cout << "\t" << (char) (j + MINCH) << " " << nodes[i].next[j] << "\n";
                }
            }
        }
    }
};

int main() {
    AhoCorasic ahoCorasic;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        string a;
        cin >> a;
        ahoCorasic.addWord(a);
        //ahoCorasic.print();
    }

    if (ahoCorasic.solve()) {
        cout << "TAK";
    } else {
        cout << "NIE";
    }
    cout << endl;

    return 0;
}