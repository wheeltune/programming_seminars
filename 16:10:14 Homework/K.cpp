#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

int n, m;
std::vector<std::pair<int, int> > left;
std::vector<int> right;
std::vector<std::vector<std::pair<int, int> > > leftEdges;
std::vector<std::pair<int, int> > pairToRight;
std::vector<int> location;
std::vector<std::pair<int, int> > next;
std::vector<int> numMax;
std::vector<int> used;

bool try_kuhn (int v) {
    if (used[v] == true)  {
        return false;
    }   
    used[v] = true;
    for (size_t i = 0; i < leftEdges[v].size(); ++i) {
        int to = leftEdges[v][i].first;
        if (pairToRight[to].first == -1) {
            if (numMax[v] == -1 || right[numMax[v]] < right[to]) {
                next[v].first = to;
                next[v].second = leftEdges[v][i].second;
                numMax[v] = to;
            }
        } else if (try_kuhn (pairToRight[to].first)) {
            if (numMax[v] == -1 || right[numMax[v]] < right[numMax[pairToRight[to].first]]) {
                next[v].first = to;
                next[v].second = leftEdges[v][i].second;
                numMax[v] = numMax[pairToRight[to].first];
            }
        }
    }
    if (numMax[v] != -1) {
        return true;
    } else {
        return false;
    }
}

int main() {
    int k;
    std::cin >> n >> m >> k;
    right.resize(m);
    leftEdges.resize(n);
    pairToRight.resize(m, std::make_pair(-1, -1));
    location.resize(n);
    next.resize(n, std::make_pair(-1, -1));
    numMax.resize(n, -1);
    used.resize(n, false);

    for (int i = 0; i < n; ++i) {
        int a;
        std::cin >> a;
        left.push_back(std::make_pair(a, i));
    }
    std::sort(left.begin(), left.end());
    std::reverse(left.begin(), left.end());
    for (int i = 0; i < n; ++i) {
        location[left[i].second] = i;
    }
    for (int i = 0; i < m; ++i) {
        std::cin >> right[i];
    }

    for (int i = 0; i < k; ++i) {
        int from, to;
        std::cin >> from >> to;
        from--;to--;
        leftEdges[location[from]].push_back(std::make_pair(to, i + 1));
    }

    for (int i = 0; i < n; ++i) {
        if (try_kuhn (i)) {
            int j = i;
            while (pairToRight[next[j].first].first != -1) {

                int k = pairToRight[next[j].first].first;
                pairToRight[next[j].first] = std::make_pair(j, next[j].second);
                j = k;
            }
            pairToRight[next[j].first] = std::make_pair(j, next[j].second);

            next.assign(n, std::make_pair(-1, -1));
            numMax.assign(n, -1);
            used.assign(n, -1);
        }
    }

    int cntAns = 0;
    int ans = 0;
    for (int i = 0; i < m; ++i) {
        if (pairToRight[i].first != -1) {
            ans += right[i];
            ans += left[pairToRight[i].first].first;
            cntAns++;
        }
    }

    std::cout << ans << "\n" << cntAns << "\n";

    for (int i = 0; i < m; ++i) {
        if (pairToRight[i].first != -1) {
            std::cout << pairToRight[i].second << " ";
        }
    }
    std::cout << "\n";
    
    return 0;
}