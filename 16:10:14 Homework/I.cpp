#include <vector>
#include <algorithm>
#include <iostream>

const int INF = 0x3fffffff;

struct Edge
{
    int to, back;           
    int flow, cap, cost;
    bool canUse;            

    Edge(int to, int back, int cap, int cost) : to(to), back(back), flow(0), cap(cap), cost(cost), canUse(true) {}
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

    void addEdge(int from, int to, int cap, int cost)
    {
        g[from].push_back(Edge(to,   g[to]  .size(),     cap, cost));
        g[to]  .push_back(Edge(from, g[from].size() - 1, 0,  -cost));
    }

    std::pair<int, int> minCostMaxFlow(int source, int sink)
    {
        std::fill(phi.begin(), phi.end(), INF);

        phi[source] = 0;
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(phi[j] < INF) {
                    for(int k = 0; k < g[j].size(); ++k) {
                        if (g[j][k].canUse) {
                            if(g[j][k].cap > 0 && phi[g[j][k].to] > phi[j] + g[j][k].cost) {
                                phi[g[j][k].to] = phi[j] + g[j][k].cost;
                            }
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
                    if (g[from][i].canUse) {
                        if(g[from][i].cap > g[from][i].flow) {
                            int to = g[from][i].to;
                            if(!used[to] && distance[to] > distance[from] + g[from][i].cost + phi[from] - phi[to]) {
                                distance[to] = distance[from] + g[from][i].cost + phi[from] - phi[to];
                                parent[to] = g[from][i].back;
                            }
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

    void clearFlow() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < g[i].size(); ++j) {
                g[i][j].flow = 0;
            }
        }
    }
};

std::vector<std::vector<int> > input;


int main() {
    int n, k;
    std::cin >> n >> k;
    Network network(2 * n + 2);

    input.resize(n);
    for (int i = 0; i < n; ++i) {
        input[i].resize(n);
        for (int j = 0; j < n; ++j) {
            std::cin >> input[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        network.addEdge(2 * n, i, k, 0);
        network.addEdge(i + n, 2 * n + 1, k, 0);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            network.addEdge(i, j + n, 1, input[i][j]);
        }
    }

    std::pair<int, int> ans = network.minCostMaxFlow(2 * n, 2 * n + 1);
    std::cout << ans.second << std::endl;

    for (int i = 0; i < network.g[2 * n].size(); ++i) {
        network.g[2 * n][i].cap = 1;
    }
    for (int i = 0; i < network.g[2 * n + 1].size(); ++i) {
        int to  = network.g[2 * n + 1][i].to;
        int num = network.g[2 * n + 1][i].back;
        network.g[to][num].cap = 1;
    }
    for (int v = 0; v < n; ++v) {
        for (int i = 0; i < network.g[v].size(); ++i) {
            int to  = network.g[v][i].to;
            int num = network.g[v][i].back;

            if (network.g[v][i].canUse && network.g[v][i].cap > 0 && network.g[v][i].flow == 0) {

                network.g[v][i].canUse = false;
                network.g[to][num].canUse = false;
            }
        }
    }
    network.clearFlow();

    for (int i = 0; i < k; ++i) {
        network.minCostMaxFlow(2 * n, 2 * n + 1);


        for (int i = 0; i < network.g[2 * n].size(); ++i) {
            int to  = network.g[2 * n][i].to;
            int num = network.g[2 * n][i].back;

            network.g[2 * n][i].flow = 0;
            network.g[to][num].flow = 0;
        }
        for (int i = 0; i < network.g[2 * n + 1].size(); ++i) {
            int to  = network.g[2 * n + 1][i].to;
            int num = network.g[2 * n + 1][i].back;

            network.g[2 * n + 1][i].flow = 0;
            network.g[to][num].flow = 0;
        }
        for (int v = 0; v < n; ++v) {
            for (int i = 0; i < network.g[v].size(); ++i) {
                int to  = network.g[v][i].to;
                int num = network.g[v][i].back;

                if (network.g[v][i].canUse && network.g[v][i].flow > 0) {
                    std::cout << network.g[v][i].to - n + 1 << " ";

                    network.g[v][i].canUse = false;
                    network.g[to][num].canUse = false;
                }
            }
        }

        std::cout << std::endl;
    }

    return 0;
}
