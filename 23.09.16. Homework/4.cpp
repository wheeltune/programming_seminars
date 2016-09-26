#include <iostream>
#include <vector>
#include <queue>

class Network {
private:
    struct Edge;

    int vertexCount;
    int source, target;

    std::vector<std::vector<Edge> > network;
    std::vector<int> distance;
    std::vector<int> prev;

    int maxFlow;

public:
    Network (int vertexCount, int source, int target) : 
        vertexCount (vertexCount), source (source), target (target)
    {
        maxFlow = 0;
        distance.resize (vertexCount);
        prev.resize (vertexCount);
        network.resize (vertexCount);
        for (auto& n_i : network) {
            n_i.resize (vertexCount, Edge (0));
        }
    }

    void addEdge (int from, int to, int capasity) 
    {
        network[from][to].capasity = capasity;
    }

    int getMaxFlow () 
    {
        while (int pushed = pushFlow (source, target)) {
            maxFlow += pushed;
        }
        return maxFlow;
    }

    void decrease (int from, int to) 
    {
        if (!network[from][to].isFull()) {
            network[from][to].capasity--;
        } else {
            if (pushFlow (from , to) == 0) {
                maxFlow--;
                pushFlow (target, to, 1);
                
                pushFlow (from, source, 1);
                addFlow (to, from, 1);
            }
            network[from][to].capasity--;
        }
    }

    void increase (int from, int to) 
    {
        network[from][to].capasity++;
        maxFlow += pushFlow (source, target);
    }

private:
    struct Edge {
        int flow, capasity;

        Edge (int capasity) : 
            capasity (capasity), flow (0) 
        {}

        int getRest () {
            return capasity - flow;
        }
        bool isFull () {
            return getRest() == 0;
        }
    };

    void addFlow (int from, int to, int flow) {
        network[from][to].flow += flow;
        network[to][from].flow -= flow;
    }

    int pushFlow (int s, int t, int flow = -1) 
    {
        std::queue<int> queue;
        for (int i = 0; i < vertexCount; ++i) {
            distance[i] = -1;
            prev[i] = -1;
        }
        distance[s] = 0;
        queue.push(s);

        while (!queue.empty() && prev[t] == -1) {
            int from = queue.front();
            queue.pop();

            for (int to = 0; to < vertexCount; ++to) {
                if (distance[to] == -1 && !network[from][to].isFull()) {
                    prev[to] = from;
                    distance[to] = distance[from] + 1;

                    if (to == t) {
                        break;
                    }

                    queue.push(to);
                }
            }
        }

        int minRest = 0;
        if (prev[t] != -1) {
            minRest = network[prev[t]][t].getRest();
            int curr = prev[t];
            while (curr != s) {
                minRest = std::min (minRest, network[prev[curr]][curr].getRest());
                curr = prev[curr];
            }

            if (flow != -1) {
                minRest = std::min (minRest, flow);
            }

            curr = t;
            while (curr != s) {
                addFlow (prev[curr], curr, minRest);
                curr = prev[curr];
            }
        }

        return minRest;
    }

};

int main ()
{
    int m, n;
    std::cin >> n >> m;

    Network network (n, 0, n - 1);

    for (int i = 0; i < m; ++i) {
        int from, to, c;
        std::cin >> from >> to >> c;
        from--; to--;

        network.addEdge(from, to, c);
    }

    std::cout << network.getMaxFlow () << std::endl;

    int k;
    std::cin >> k;
    for (int i = 0; i < k; ++i) {
        int type, from, to;
        std::cin >> type >> from >> to;
        from--; to--;

        if (type == 1) {
            network.increase (from, to);
        } else {
            network.decrease (from, to);
        }
        std::cout << network.getMaxFlow () << std::endl;
    }
    return 0;
}