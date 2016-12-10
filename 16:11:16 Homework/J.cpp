#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

const int ALPH = 26;
const char MINCH = 'a';

struct Node {
    Node() : Node(-1, -1) {}

    Node(int parent, int pChar) : parent(parent), pChar(pChar) {
        for (int i = 0; i < ALPH; ++i) {
            next[i] = -1;
            edge[i] = -1;
        }
        link = -1;
        termCount = 0;
        ans = -1;

        isScanned = false;
    }

    int next[ALPH];
    int edge[ALPH];
    int link;
    int parent;
    int pChar;
    int termCount;
    long long ans;

    bool isScanned;
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

    long long getAns(int pos) {
        if (nodes[pos].ans == -1) {
            if (pos == 0) {
                nodes[pos].ans = 0;
            } else {
                nodes[pos].ans = getAns(getLink(pos));
                nodes[pos].ans += nodes[pos].termCount;
            }
        }
        return nodes[pos].ans;
    }

public:
    AhoCorasic() {
        nodes.push_back(Node(0, false));
    }

    AhoCorasic(istream &in) {
        int n;
        in >> n;
        nodes.resize(n);
        for (int i = 0; i < n; ++i) {
            int cnt = 0;
            in >> cnt;
            for (int j = 0; j < cnt; ++j) {
                char c;
                int child;
                in >> child >> c;
                child--;

                nodes[i].next[c - MINCH] = child;
                nodes[child].parent = i;
                nodes[child].pChar = c - MINCH;
            }
            nodes[i].isScanned = true;
        }
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
        nodes[pos].termCount++;
    }

    long long solve() {
        long long ans = 0;
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i].isScanned) {
                ans += getAns(i);
            }
        }
        return ans;
    }

    void print() {
        for (int i = 0; i < nodes.size(); ++i) {
            cout << i << " p: " << nodes[i].parent << " c: " << (char) nodes[i].pChar << " t: " << nodes[i].termCount << "\n";
            for (int j = 0; j < ALPH; ++j) {
                if (nodes[i].next[j] != -1) {
                    cout << "\t" << (char) (j + MINCH) << " " << nodes[i].next[j] << "\n";
                }
            }
        }
    }
};

int main() {
    AhoCorasic ahoCorasic(cin);
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        string str;
        cin >> str;
        ahoCorasic.addWord(str);
    }

    cout << ahoCorasic.solve() << endl;

    return 0;
}