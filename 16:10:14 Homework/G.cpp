#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

const int INF = 2 * 1000 * 1000 * 1000 + 1;


struct Edge {
    Edge (int from, int to, int capasity, int cost, int id) : from(from), to(to), capasity(capasity), cost(cost), flow(0), id(id) {}
    int cost, flow, capasity, from, to, id;
};

std::vector<Edge> edges;
std::vector<std::vector<int> > g;

int n, m, k;

void addEdge (int from, int to, int capasity, int cost, int id) {
    g[from].push_back(edges.size());
    edges.push_back(Edge (from, to, capasity, cost, id));
    g[to].push_back(edges.size());
    edges.push_back(Edge (to, from, 0, -cost, id));
}

std::pair<int, int> minCostMaxFlow () {
    int flow = 0, cost = 0;
    int s = 0, t = n - 1;

    while (flow < k) {
        std::vector<int> color    (n, 0);
        std::vector<int> distance (n, INF);
        std::vector<int> queue    (n);
        std::vector<int> parent   (n);
        int qb = 0, qe = 0;

        queue[qe++] = s;
        distance[s] = 0;
        while (qb != qe) {
            int v = queue[qb++];

            color[v] = 2;
            if (qb == n) qb = 0;
            for (std::size_t i = 0; i < g[v].size(); ++i) {
                Edge e = edges[g[v][i]];
                if (e.flow < e.capasity && distance[v] + e.cost < distance[e.to]) {
                    distance[e.to] = distance[v] + e.cost;
                    if (color[e.to] == 0) {
                        queue[qe++] = e.to;
                        if (qe == n) qe = 0;
                    } else {
                        if (--qb == -1) qb = n - 1;
                        queue[qb] = e.to;
                    }
                    color[e.to] = 1;
                    parent[e.to] = g[v][i];
                }
            }
        }

        if (distance[t] == INF) break;
        int addFlow = k - flow;
        for (int v = t; v != s; v = edges[parent[v]].from) {
            addFlow = std::min (addFlow, edges[parent[v]].capasity - edges[parent[v]].flow);
        }
        for (int v = t; v != s; v = edges[parent[v]].from) {
            edges[parent[v]].flow += addFlow;
            edges[parent[v]^1].flow -= addFlow;
            cost += edges[parent[v]].cost * addFlow;
        }
        flow += addFlow;
    }
    return std::make_pair(flow, cost);
}

std::vector<int> findWay() {
    int s = 0, t = n - 1;

    std::vector<int> ans;

    std::vector<int> color    (n, 0);
    std::vector<int> distance (n, INF);
    std::vector<int> queue    (n);
    std::vector<int> parent   (n);
    int qb = 0, qe = 0;

    queue[qe++] = s;
    distance[s] = 0;
    while (qb != qe) {
        int v = queue[qb++];

        color[v] = 2;
        if (qb == n) qb = 0;
        for (std::size_t i = 0; i < g[v].size(); ++i) {
            Edge e = edges[g[v][i]];
            if (e.flow > 0 && e.flow == e.capasity && distance[v] + 1 < distance[e.to]) {
                distance[e.to] = distance[v] + 1;
                if (color[e.to] == 0) {
                    queue[qe++] = e.to;
                    if (qe == n) qe = 0;
                } else {
                    if (--qb == -1) qb = n - 1;
                    queue[qb] = e.to;
                }
                color[e.to] = 1;
                parent[e.to] = g[v][i];
            }
        }
    }

    for (int v = t; v != s; v = edges[parent[v]].from) {
        edges[parent[v]].flow--;
        ans.push_back (edges[parent[v]].id);
    }
    std::reverse(ans.begin(), ans.end());

    return ans;
}

int main() {
    std::cin >> n >> m >> k;

    g.resize(n);
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        std::cin >> from >> to >> cost;
        from--; to--;
        addEdge (from, to, 1, cost, i + 1);
        addEdge (to, from, 1, cost, i + 1);
    }

    std::cout.flush();

    std::pair<int, int> ans = minCostMaxFlow();
    if (ans.first < k) {
        std::cout << "-1\n";
    } else {
        printf ("%.6lf\n", ((double)ans.second) / k);
        for (int i = 0; i < k; ++i) {
            std::vector<int> way = findWay();
            std::cout << way.size();
            for (int j = 0; j < way.size(); ++j) {
                std::cout << " " << way[j];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}