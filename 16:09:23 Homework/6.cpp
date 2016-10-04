#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int> > graph;
std::vector<int> vPair;
std::vector<bool> isUsed;
std::vector<bool> isRight; 
int n, m;

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

    vPair.assign (m, -1);
    isUsed.assign (n, 0);
    for (int i = 0; i < n; ++i) {
        if (pairBfs (i)) {
            isUsed.assign (n, 0);
        }
    }

    std::vector<bool> isFree (n, true);
    for(int i = 0; i < m; i++) {
        if(vPair[i] != -1) {
            isFree[vPair[i]] = false;
        }
    }

    isRight.assign (m, false);
    isUsed.assign  (n, false);
    for(int i = 0; i < n; ++i) {
        if (isFree[i]) {
            dominatingBfs (i);
        }
    }

    std::pair<std::vector<int>, std::vector<int> > res;
    for (int i = 0; i < m; ++i) {
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

void create() {
    graph.resize(n);
    vPair.resize(m);
    isRight.resize(m);
    isUsed.resize(n);
}

void destroy() {
    for (int i = 0; i < n; ++i) {
        graph[i].clear();
    }
}

int main() 
{
    int k;
    std::cin >> k;
    while (k > 0) {
        std::cin >> n >> m;

        create();

        std::vector<bool> has(m, true);
        for (int i = 0; i < n; ++i) {
            int a;
            std::cin >> a;
            while (a > 0) {
                a--;
                has[a] = false;
                std::cin >> a;
            }

            for (int j = 0; j < m; ++j) {
                if (has[j]) {
                    graph[i].push_back(j);
                }
            }

            has.assign(m, true);
        }

        std::pair<std::vector<int>, std::vector<int> > v = control();
        std::cout << m + n - (v.first.size() + v.second.size()) << "\n";
        std::cout << n - v.first.size() << " " << m - v.second.size() << "\n";
        int j = 0;
        for (int i = 0; i < n; ++i) {
            if ((j < v.first.size()) && v.first[j] == i) {
                j++;
            } else {
                std::cout << i + 1 << " ";
            }
        }
        std::cout << "\n";

        j = 0;
        for (int i = 0; i < m; ++i) {
            if ((j < v.second.size()) && v.second[j] == i) {
                j++;
            } else {
                std::cout << i + 1 << " ";
            }
        }

        std::cout << "\n\n";

        destroy();

        k--;
    }
    return 0;
}