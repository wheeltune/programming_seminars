#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

int n, m;

std::vector<std::vector<int> > leftEdges;
std::vector<std::vector<int> > rightEdges;
std::vector<int> pairToRight;
std::vector<int> pairToLeft;
std::vector<int> used;
int cleaner;

int l, r;
int pairCnt;

bool try_back_kuhn (int v) {
    if (used[v] == cleaner) {
        return false;
    }
    used[v] = cleaner;
    for (size_t i = 0; i < rightEdges[v].size(); ++i) {
        int to = rightEdges[v][i];
        if (pairToLeft[to] == -1 || try_back_kuhn (pairToLeft[to])) {
            pairToLeft[to] = v;
            pairToRight[v] = to;
            return true;
        }
    }
    return false;
}

bool try_kuhn (int v) {
    if (used[v] == cleaner)  {
        return false;
    }
    used[v] = cleaner;
    for (size_t i = 0; i < leftEdges[v].size(); ++i) {
        int to = leftEdges[v][i];
        if (to < l || to >= r) continue;
        if (pairToRight[to] == -1 || try_kuhn (pairToRight[to])) {
            pairToRight[to] = v;
            pairToLeft[v] = to;
            return true;
        }
    }
    return false;
}

void add() {
    cleaner++;
    while (r < n && pairCnt < m) {
        if (try_back_kuhn(r)) {
            cleaner++;
            pairCnt++;
        }
        r++;
    }
}

int main() {
    int k;
    std::cin >> n >> m >> k;
    leftEdges.resize(m);
    rightEdges.resize(n);
    pairToRight.resize(n, -1);
    pairToLeft.resize(m, -1);
    used.resize(std::max(m, n), 0);

    for (int i = 0; i < k; ++i) {
        int a, b;
        std::cin >> a >> b;
        a--; b--;
        leftEdges[b].push_back(a);
        rightEdges[a].push_back(b);
    }

    int ans = 0;
    l = 0;
    r = 0;

    add();
    for (; pairCnt == m && l < n; ++l) {
        ans += n - r + 1;
        if (pairToRight[l] != -1) {
            pairToLeft[pairToRight[l]] = -1;

            cleaner++;
            if (!try_kuhn(pairToRight[l])) {
                pairCnt--;
                add();
            }
        }
    }

    std::cout << ans << std::endl;

    return 0;
}