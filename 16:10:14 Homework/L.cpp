#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int> > graph;
std::vector<int> vPair;
std::vector<bool> isUsed;
std::vector<bool> isRight; 
int n;

bool pairBfs (int from) {
    if (isUsed[from]) return false;
    isUsed[from] = true;
    for (std::size_t i = 0; i < graph[from].size(); ++i) {
        int to = graph[from][i];
        if (vPair[to] == -1 || pairBfs(vPair[to])) {
            vPair[to] = from;
            return true;
        }
    }
    return false;
}

void dominatingBfs (int from) 
{
    isUsed[from] = true;
    for (std::size_t i = 0; i < graph[from].size(); ++i) {
        int to = graph[from][i];
        if (!isRight[to] && !isUsed[vPair[to]]) {
            dominatingBfs (vPair[to]);
            isRight[to] = true;
        }
    }
}

std::pair<std::vector<int>, std::vector<int> > control ()
{
    int i = 0;

    vPair.assign (n, -1);
    isUsed.assign (n, 0);
    for (int i = 0; i < n; ++i) {
        if (pairBfs (i)) {
            isUsed.assign (n, 0);
        }
    }

    std::vector<bool> isFree (n, true);
    for(int i = 0; i < n; i++) {
        if(vPair[i] != -1) {
            isFree[vPair[i]] = false;
        }
    }

    isRight.assign (n, false);
    isUsed.assign  (n, false);
    for(int i = 0; i < n; ++i) {
        if (isFree[i]) {
            dominatingBfs (i);
        }
    }

    std::pair<std::vector<int>, std::vector<int> > res;
    for (int i = 0; i < n; ++i) {
        if (vPair[i] != -1) {
            if (isRight[i]) {
                res.second.push_back (i);
            } else {
                res.first.push_back (vPair[i]);
            }
        }
    }

    std::sort (res.first.begin(), res.first.end());

    return res;
}

std::vector<std::vector<int> > g;

int main() 
{
    std::cin >> n;

    graph.resize(n);
    vPair.resize(n);
    isRight.resize(n);
    isUsed.resize(n);
    g.resize(n);

    for (int i = 0; i < n; ++i) {
        g[i].resize(n, 0);
        for (int j = 0; j < n; ++j) {
            std::cin >> g[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (g[i][k] == 1 && g[k][j] == 1) {
                    g[i][j] = 1;
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g[i][j] == 1) {
                graph[i].push_back(j);
            }
        }
    }

    std::pair<std::vector<int>, std::vector<int> > v = control();
    

    std::vector<int> ans(n, 0);
    for (int i = 0; i < v.first.size(); ++i) {
        ans[v.first[i]] ++;
    }
    for (int i = 0; i < v.second.size(); ++i) {
        ans[v.second[i]] ++;
    }

    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        if (ans[i] == 0) {
            cnt++;
        }
    }
    std::cout << cnt << std::endl;
    for (int i = 0; i < n; ++i) {
        if (ans[i] == 0) {
            std::cout << i + 1 << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}