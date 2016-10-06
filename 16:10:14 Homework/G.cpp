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

int cost = 0;
int n, m, k;
int s, t;

void addEdge (int from, int to, int capasity, int cost, int id) {
    g[from].push_back(edges.size());
    edges.push_back(Edge (from, to, capasity, cost, id));
    g[to].push_back(edges.size());
    edges.push_back(Edge (to, from, 0, -cost, id));
}

std::vector<int> dinicDistance;
std::vector<int> dinicVPos;

bool dinicBfs ()
{
    std::queue<int> queue;

    for (auto& d_i : dinicDistance) {
        d_i = INF;
    }
    dinicDistance[s] = 0;
    queue.push (s);
    while (!queue.empty() && dinicDistance[t] == INF) {
        int from = queue.front();
        queue.pop();

        for (int i = 0; i < g[from].size(); ++i) {
            Edge e = edges[g[from][i]];
            if (dinicDistance[e.to] == INF && e.flow < e.capasity) {
                dinicDistance[e.to] = dinicDistance[e.from] + 1;
                queue.push (e.to);
            }
        }
    }
    return dinicDistance[t] != INF;
} 

int dinicDfs (int v, int flow)
{
    if (flow == 0 || v == t) return flow;
    for (; dinicVPos[v] < g[v].size(); ++dinicVPos[v]) {
        Edge e = edges[g[v][dinicVPos[v]]];

        if (dinicDistance[e.to] == dinicDistance[v] + 1) {

            int pushed = dinicDfs(e.to, std::min(flow, e.capasity - e.flow));

            if (pushed > 0) {
                edges[g[v][dinicVPos[v]]  ].flow += pushed;
                edges[g[v][dinicVPos[v]]^1].flow -= pushed;
                cost += edges[g[v][dinicVPos[v]]].cost * pushed;

                return pushed;
            }
        }
    }
    return 0;
}

int dinic ()
{
    int maxFlow = 0;
    while (dinicBfs()) {
        
        for (int i = 0; i < n; ++i) {
            dinicVPos[i] = 0;
        }
        while (int pushed = dinicDfs (s, INF)) {
            maxFlow += pushed;
        }
    }
    return maxFlow;
}

void minCostMaxFlow () {

    bool found;
    do {
        found = false;
 
        std::vector<int> distance (n, INF);
        std::vector<int> parent   (n, -1);

        for (int i = 0; i < n; ++i) {
            if (distance[i] == INF) {

                distance[i] = 0;
                std::vector<int> list, sList;

                list.push_back (i);
                for (int r = 0; r < n && !list.empty(); ++r) {
                    sList.clear();
                    std::sort (list.begin(), list.end());
                    list.erase (std::unique (list.begin(), list.end()), list.end());

                    for (size_t j = 0; j < list.size(); ++j) {
                        int v = list[j];
                        for (size_t k = 0; k < g[v].size(); ++k) {
                            Edge e = edges[g[v][k]];

                            if (e.flow < e.capasity && distance[v] + e.cost < distance[e.to]) {
                                distance[e.to] = distance[v] + e.cost;
                                parent[e.to] = g[v][k];
                                sList.push_back (e.to);
                            }
                                
                        }
                    }
                    std::swap (list, sList);
                }
                
                if (!list.empty()) {
                    int leaf = list[0];

                    std::vector<int> pathEdges;
                    std::vector<int> path;

                    int v;
                    for (v = leaf; parent[v] != -1; v = edges[parent[v]].from) {
                        auto pos = std::find (path.begin(), path.end(), v);
                        if (pos == path.end()) {
                            path.push_back (v);
                            pathEdges.push_back (parent[v]);
                        } else {
                            path.erase (path.begin(), pos);
                            pathEdges.erase (pathEdges.begin(), pathEdges.begin() + (pathEdges.size() - path.size()));
                            break;
                        }
                    }
                    if (parent[v] == -1) {
                        auto pos = std::find (path.begin(), path.end(), v);
                        if (pos == path.end()) {
                            path.push_back (v);
                            pathEdges.push_back (parent[v]);
                        } else {
                            path.erase (path.begin(), pos);
                            pathEdges.erase (pathEdges.begin(), pathEdges.begin() + (pathEdges.size() - path.size()));
                            break;
                        }
                    }
                        
                    for (size_t j = 0; j < pathEdges.size(); ++j) {
                        edges[pathEdges[i]].flow += 1;
                        edges[pathEdges[i]^1].flow -= 1;
                        cost += edges[pathEdges[i]].cost;
                    }
                    found = true;
                }
            }
        }
    } while (found);
}

int lastWayCost = 0;

std::vector<int> findWay() {
    int s = 0, t = n - 1;
    lastWayCost = 0;

    std::vector<int> ans;

    std::vector<int> color    (n, 0);
    std::vector<int> distance (n, INF);
    std::vector<int> parent   (n);

    distance[s] = 0;
    for (int cnt = 0; cnt < n; ++cnt) {
        int v = t;
        for (int i = 0; i < n; ++i) {
            if (color[i] == 0 && distance[i] < distance[v]) {
                v = i;
            }
        }

        color[v] = 1;
        for (int i = 0; i < g[v].size(); ++i) {
            Edge e = edges[g[v][i]];
            if (e.flow > 0 && e.flow == e.capasity && distance[v] + e.cost < distance[e.to]) {
                distance[e.to] = distance[v] + e.cost;
                parent[e.to] = g[v][i];
            }
        }
    }

    for (int v = t; v != s; v = edges[parent[v]].from) {
        edges[parent[v]].flow--;
        lastWayCost += edges[parent[v]].cost;
        ans.push_back (edges[parent[v]].id);
    }
    std::reverse(ans.begin(), ans.end());

    return ans;
}

int main() {
    std::cin >> n >> m >> k;
    s = 0, t = n - 1;

    g.resize(n);
    dinicVPos.resize(n, 0);
    dinicDistance.resize(n, INF);
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        std::cin >> from >> to >> cost;
        from--; to--;
        addEdge (from, to, 1, cost, i + 1);
        addEdge (to, from, 1, cost, i + 1);
    }

    if (dinic() < k) {
        std::cout << "-1\n";
    } else {
        int sumCost = 0;
        minCostMaxFlow();
        std::vector<std::vector<int> > ways;
        for (int i = 0; i < k; ++i) {
            ways.push_back(findWay());
            sumCost += lastWayCost;
        }
        
        printf ("%.6lf\n", ((double)sumCost) / k);
        for (int i = 0; i < k; ++i) {
            std::cout << ways[i].size();
            for (int j = 0; j < ways[i].size(); ++j) {
                std::cout << " " << ways[i][j];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}