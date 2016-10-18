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


std::vector<std::pair<std::pair<int, int>, std::pair<int, int> > > input;

int main() {
    int n, k;
    std::cin >> n >> k;
    Network network(2 * n + 2);

    input.resize(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> input[i].first.first >> input[i].first.second >> input[i].second.first;
        input[i].second.second = i;
    }
    std::sort(input.begin(), input.end());

    network.addEdge(2 * n, 0, k, 0);
    for (int i = 0; i < n - 1; ++i) {
        network.addEdge(i, i + 1, INF, 0);
    }
    for (int i = 0; i < n; ++i) {
        network.addEdge(i, n + i, 1, -input[i].second.first);
        int ind = -1;
        for (int j = i + 1; j < n; ++j) {
            if (input[i].first.first + input[i].first.second <= input[j].first.first) {
                ind = j;
                break;
            }
        }
        if (ind != -1) {
            network.addEdge(i + n, ind, 1, 0);
        }
    }
    for (int i = 0; i < n; ++i) {
        network.addEdge(i + n, 2 * n + 1, 1, 0);
    }

    std::pair<int, int> p = network.minCostMaxFlow(2 * n, 2 * n + 1);

    std::vector<int> ans(n);

    for (int i = 0; i < n; ++i) {
        ans[input[i].second.second] = (int)(network.g[n + i][0].flow < 0);
    }
    for (int i = 0; i < n; ++i) {
        std::cout << ans[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
