#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Bor {
private:
    std::vector<int*> edges;
    std::vector<bool> isTerm;
    std::vector<int> count;

    int* getEmptyEdges() {
        int* emptyEdges = new int[32];
        for (int i = 0; i < 32; ++i) {
            emptyEdges[i] = -1;
        }
        return emptyEdges;
    }
public:
    Bor() {
        edges.push_back(getEmptyEdges());
        isTerm.push_back(false);
        count.push_back(0);
    }

    ~Bor() {
        for (int i = 0; i < edges.size(); ++i) {
            free(edges[i]);
        }
        edges.clear();
        isTerm.clear();
        count.clear();
    }

    string getKStat(int k) {
        string ans = "";
        int pos = 0;
        while (k > 1 || !isTerm[pos]) {
            if (isTerm[pos]) {
                k--;
            }
            for (int i = 0; i < 32; ++i) {
                if (edges[pos][i] == -1) continue;

                if (count[edges[pos][i]] < k) {
                    k -= count[edges[pos][i]];
                } else {
                    ans += (char) ('a' + i);
                    pos = edges[pos][i];
                    break;
                }
            }
        }
        return ans;
    }

    void addString(const string &str) {
        int pos = 0;
        for (int sPos = 0; sPos < str.length(); ++sPos) {
            count[pos]++;
            int newPos = edges[pos][str[sPos] - 'a'];
            if (newPos == -1) {
                edges[pos][str[sPos] - 'a'] = edges.size();
                newPos = edges[pos][str[sPos] - 'a'];
                edges.push_back(getEmptyEdges());
                isTerm.push_back(false);
                count.push_back(0);
            }
            pos = newPos;
        }
        isTerm[pos] = true;
        count[pos]++;
    }

    void print() {
        for (int i = 0; i < edges.size(); ++i) {
            printf("%2d (%2d, %d): ", i, count[i], (int)isTerm[i]);
            for (int j = 0; j < 32; ++j) {
                if (edges[i][j] != -1) {
                    printf("%c (%2d) ", (char)('a' + j), edges[i][j]);
                }
            }
            printf("\n");
        }
    }
};

int toInt(string &s) {
    int ans = 0;
    for (int i = 0; i < s.length(); ++i) {
        ans *= 10;
        ans += s[i] - '0';
    }
    return ans;
}

int main() {
    string s;
    Bor bor;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> s;
        if (s[0] >= '0' && s[0] <= '9') {
            int k = toInt(s);

            std::cout << bor.getKStat(k) << '\n';
        } else {
            bor.addString(s);
        }
    }
    return 0;
}