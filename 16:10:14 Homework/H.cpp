#include <vector>
#include <algorithm>
#include <iostream>

const int INF = 0x3fffffff;

struct Edge
{
    int to, back;           
    int flow, cap, cost;            

    Edge(int to, int back, int cap, int cost) : to(to), back(back), flow(0), cap(cap), cost(cost) {}
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

                for(int i = 0; i < g[i].size(); ++i) {
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

std::vector<std::vector<int> > input;

int main() {
    int n;
    std::cin >> n;
    Network network(2 * n + 2);

    input.resize(n);
    for (int i = 0; i < n; ++i) {
        input[i].resize(n);
        for (int j = 0; j < n; ++j) {
            std::cin >> input[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        network.addEdge(2 * n, i, 1, 0);
        network.addEdge(i + n, 2 * n + 1, 1, 0);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            network.addEdge(i, j + n, 1, input[i][j]);
        }
    }

    std::pair<int, int> ans = network.minCostMaxFlow(2 * n, 2 * n + 1);
    std::cout << ans.second << std::endl;

    for (int i = 0; i < network.g[2 * n].size(); ++i) {
        int x, y;
        x = network.g[2 * n][i].to;
        for (int j = 0; j < network.g[x].size(); ++j) {
            if (network.g[x][j].flow > 0) {
                y = network.g[x][j].to - n;
                break;
            }
        }

        std::cout << x + 1 << " " << y + 1 << std::endl;
    }

    return 0;
}
