#include <vector>
#include <algorithm>
#include <iostream>

const int INF = 0x3fffffff;

struct Edge
{
    int to, back;           
    int flow, cap, cost; 
    int id;           

    Edge(int to, int back, int cap, int cost, int id) : to(to), back(back), flow(0), cap(cap), cost(cost), id(id) {}
};

struct Network
{
    int n;             
    std::vector<std::vector<Edge> > g;    
    std::vector<int> phi;       
    std::vector<int> distance;          
    std::vector<int> parent; 
    std::vector<bool> used;        

    Network(int n) : n(n), g(n), phi(n), distance(n), used(n), parent(n) {}

    void addEdge(int from, int to, int cap, int cost, int id)
    {
        g[from].push_back(Edge(to,   g[to]  .size(),     cap, cost, id));
        g[to]  .push_back(Edge(from, g[from].size() - 1, 0,  -cost, id));
    }

    std::pair<int, int> minCostMaxFlow (int source, int sink)
    {
        std::fill(phi.begin(), phi.end(), INF);

        phi[source] = 0;
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(phi[j] < INF) {
                    for(int k = 0; k < g[j].size(); ++k) {
                        if(g[j][k].cap > 0 && phi[g[j][k].to] > phi[j] + g[j][k].cost) {
                            phi[g[j][k].to] = phi[j] + g[j][k].cost;
                        }
                    }
                }
            }
        }

        int resultFlow = 0, resultCost = 0;

        while (true)
        {
            std::fill(used.begin(), used.end(), false);
            std::fill(distance.begin(), distance.end(), INF);

            distance[source] = 0;

            while (true)
            {
                int from = -1;

                for(int i = 0; i < n; ++i) {
                    if(!used[i] && distance[i] < INF && (from < 0 || distance[from] > distance[i])) {
                        from = i;
                    }
                }

                if(from < 0) break;

                used[from] = 1;

                for(int i = 0; i < g[from].size(); ++i) {
                    if(g[from][i].cap > g[from][i].flow) {
                        int to = g[from][i].to;
                        if(!used[to] && distance[to] > distance[from] + g[from][i].cost + phi[from] - phi[to]) {
                            distance[to] = distance[from] + g[from][i].cost + phi[from] - phi[to];
                            parent[to] = g[from][i].back;
                        }
                    }
                }
            }

            if(!used[sink]) break;

            for(int i = 0; i < n; ++i) {
                phi[i] += used[i] ? distance[i] : distance[sink];
            }

            int augFlow = INF, augCost = 0;
            for(int i = sink; i != source; )
            {
                int to  = g[i][parent[i]].to;
                int num = g[i][parent[i]].back;

                augFlow = std::min(augFlow, g[to][num].cap - g[to][num].flow);
                augCost += g[to][num].cost;

                i = to;
            }

            for(int i = sink; i != source; )
            {
                int to  = g[i][parent[i]].to;
                int num = g[i][parent[i]].back;

                g[to][num].flow += augFlow;
                g[i][parent[i]].flow -= augFlow;

                i = to;
            }

            resultFlow += augFlow;
            resultCost += augFlow * augCost;
        }

        return std::make_pair(resultFlow, resultCost);
    }
};

int n, m, k;

std::vector<int> findWay(Network &network, int source, int sink) {
    std::vector<int> ans;

    std::vector<int> color    (n, 0);
    std::vector<int> distance (n, INF);
    std::vector<int> parent   (n);

    distance[source] = 0;
    while (color[sink] == 0) {
        int v = sink;
        for (int i = 0; i < n; ++i) {
            if (color[i] == 0 && distance[i] < distance[v]) {
                v = i;
            }
        }

        if (color[v] == 1) break;

        color[v] = 1;
        for (int i = 0; i < network.g[v].size(); ++i) {
            Edge e = network.g[v][i];
            if (e.flow > 0 && distance[v] + e.cost < distance[e.to]) {
                distance[e.to] = distance[v] + e.cost;
                parent[e.to] = e.back;
            }
        }
    }

    for (int i = sink; i != source; ) {
        int to  = network.g[i][parent[i]].to;
        int num = network.g[i][parent[i]].back;

        ans.push_back (network.g[to][num].id);
        network.g[to][num].flow      -= 1;
        network.g[i][parent[i]].flow += 1;

        i = to;
    }
    std::reverse(ans.begin(), ans.end());

    return ans;
}

int main() {
    std::cin >> n >> m >> k;

    n++;
    Network network(n);

    network.addEdge (n - 1, 0, k, 0, 0);

    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        std::cin >> from >> to >> cost;
        from--; to--;
        network.addEdge (from, to, 1, cost, i + 1);
        network.addEdge (to, from, 1, cost, i + 1);
    }

    std::pair<int, int> ans = network.minCostMaxFlow (n - 1, n - 2);

    if (ans.first < k) {
        std::cout << "-1\n";
    } else {
        printf ("%.6lf\n", ((double)ans.second) / k);
        
        for (int i = 0; i < k; ++i) {
            std::vector<int> way = findWay(network, n - 1, n - 2);

            std::cout << way.size() - 1;
            for (int i = 1; i < way.size(); ++i) {
                std::cout << " " << way[i];
            }
            std::cout << "\n";
        }
    }
    return 0;
}
