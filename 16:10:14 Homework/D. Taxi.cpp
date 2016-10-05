#include <iostream>
#include <cmath>
#include <vector>

struct Vertex {

    int sTime, fTime, x1, y1, x2, y2;

    Vertex() {}
    Vertex (int sTime, int x1, int y1, int x2, int y2) : sTime(sTime), fTime(sTime + std::abs(x1 - x2) + std::abs(y1 - y2)), x1(x1), y1(y1), x2(x2), y2(y2) {
    }    

    int timeTo (Vertex v) {
        return std::abs(v.x1 - x2) + std::abs(v.y1 - y2);
    }
};

std::vector<Vertex> left;
std::vector<Vertex> right;
std::vector<std::vector<int> > leftEdges;
std::vector<std::vector<int> > rightEdges;
int n;

bool isOk (Vertex v1, Vertex v2) {
    if (v1.fTime + v1.timeTo(v2) < v2.sTime) {
        return true;
    }
    return false;
}

std::vector<int> pairToRight;
std::vector<bool> used;

bool try_kuhn (int v) {
    if (used[v])  {
        return false;
    }
    used[v] = true;
    for (size_t i = 0; i < leftEdges[v].size(); ++i) {
        int to = leftEdges[v][i];
        if (pairToRight[to] == -1 || try_kuhn (pairToRight[to])) {
            pairToRight[to] = v;
            return true;
        }
    }
    return false;
}

void kuhn () {
    pairToRight.assign (n, -1);
    for (int v = 0; v < n; ++v) {
        used.assign (n, false);
        try_kuhn (v);
    }
}

int main() {
    std::cin >> n;
    leftEdges.resize(n);
    rightEdges.resize(n);
    pairToRight.resize(n);
    used.resize(n);

    for (int i = 0; i < n; ++i) {
        std::string s;
        int x1, y1, x2, y2;
        std::cin >> s >> x1 >> y1 >> x2 >> y2;
        int time = ((s[0] - '0') * 10 + (s[1] - '0')) * 60 + ((s[3] - '0') * 10 + (s[4] - '0'));

        left.push_back  (Vertex (time, x1, y1, x2, y2));
        right.push_back (Vertex (time, x1, y1, x2, y2));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (isOk(right[i], left[j])) {
                rightEdges[i].push_back(j);
            } else if (isOk (left[j], right[i])) {
                leftEdges[j].push_back(i);
            }
        }
    }
    kuhn();

    int m = 0;
    for (int i = 0; i < n; ++i) {
        if (pairToRight[i] != -1) {
            m++;
        }
    }
    std::cout << n - m << std::endl;

    return 0;
}