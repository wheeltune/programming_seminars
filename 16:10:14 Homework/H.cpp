#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

const int INF = 2 * 1000 * 1000 * 1000 + 1;

struct Edge {
    Edge (int from, int to, int capasity, int cost) : from(from), to(to), capasity(capasity), cost(cost), flow(0) {}
    int cost, flow, capasity, from, to;
};

std::vector<Edge> edges;
std::vector<std::vector<int> > g;

int cost = 0;
int n, m, k;
int s, t;

void addEdge (int from, int to, int capasity, int cost) {
    g[from].push_back(edges.size());
    edges.push_back(Edge (from, to, capasity, cost));
    g[to].push_back(edges.size());
    edges.push_back(Edge (to, from, 0, -cost));
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
                        path.erase (path.begin(), pos);
                        pathEdges.erase (pathEdges.begin(), pathEdges.begin() + (pathEdges.size() - path.size()));
                    }
                        
                    for (size_t j = 0; j < pathEdges.size(); ++j) {
                        edges[pathEdges[j]].flow += 1;
                        edges[pathEdges[j]^1].flow -= 1;
                        cost += edges[pathEdges[j]].cost;
                    }
                    found = true;
                }
            }
        }
    } while (found);
}

std::vector<std::vector<int> > input;

int main() {
    std::cin >> m;
    n = 2 * m + 2;

    input.resize(n);
    for (int i = 0; i < m; ++i) {
        input[i].resize(n);
        for (int j = 0; j < m; ++j) {
            std::cin >> input[i][j];
        }
    }
    s = n - 2, t = n - 1;

    g.resize(n);
    dinicVPos.resize(n, 0);
    dinicDistance.resize(n, INF);

    for (int i = 0; i < m; ++i) {
        addEdge(s, i, 1, 0);
        addEdge(i + m, t, 1, 0);
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            addEdge(i, j + m, 1, input[i][j]);
        }
    }

    dinic();
    minCostMaxFlow();
    std::cout << cost << "\n";
    for (int i = 0; i < g[s].size(); ++i) {
        int x, y;
        x = edges[g[s][i]].to;
        for (int j = 0; j < g[x].size(); ++j) {
            if (edges[g[x][j]].flow > 0) {
                y = edges[g[x][j]].to - m;
                break;
            }
        }
        std::cout << x + 1 << " " << y + 1 << "\n";
    }

    return 0;
}