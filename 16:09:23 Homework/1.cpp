#include <iostream>
#include <vector>
#include <queue>

#define INF 1000000000

class Edge {
private:
    int f_, c_;
    int from_, to_;
public:
    Edge (int from, int to, int f) : from_(from), to_(to), f_(f), c_(0) {
    }

    int getF ()
    {
        return f_;
    }
    int getC ()
    {
        return c_;
    }
    int getFrom ()
    {
        return from_;
    }
    int getTo ()
    {
        return to_;
    }

    int getRest ()
    {
        return f_ - c_;
    }

    bool isFull ()
    {
        return getRest() == 0;
    }

    bool pushFlow (int add) 
    {
        if (c_ + add > f_) return false;
        c_ += add;
        return true;
    }
};

int n, s, t;
std::vector<Edge> edges;
std::vector<std::vector<std::size_t> > network;

std::vector<std::size_t> ptr;
std::vector<int> d;

bool bfs ()
{
    std::queue<int> q;
    for (auto& d_i : d) {
        d_i = -1;
    }
    d[s] = 0;
    q.push (s);
    while (!q.empty() && d[t] == -1) {
        int from = q.front();
        q.pop();

        for (std::size_t i = 0; i < network[from].size(); ++i) {
            int id = network[from][i];
            int to = edges[id].getTo();

            if (d[to] == -1 && !edges[id].isFull()) {
                d[to] = d[from] + 1;
                q.push (to);
            }
        }
    }
    return d[t] != -1;
} 

int dfs (int v, int flow)
{
    if (flow == 0 || v == t) return flow;
    for (; ptr[v] < network[v].size(); ++ptr[v]) {
        std::size_t id = network[v][ptr[v]];

        if (d[edges[id].getTo()] == d[v] + 1) {
            int pushed = dfs(edges[id].getTo(), std::min(flow, edges[id].getRest()));

            if (pushed > 0) {
                edges[id  ].pushFlow( pushed);
                edges[id^1].pushFlow(-pushed);

                return pushed;
            }
        }
    }
    return 0;
}

int dinic ()
{
    int maxFlow = 0;
    while (bfs()) {
        for (auto& ptr_i : ptr) {
            ptr_i = 0;
        }
        while (int pushed = dfs (s, INF)) {
            maxFlow += pushed;
        }
    }
    return maxFlow;
}

void addEdge (int from, int to, int f) 
{
    edges.push_back(Edge(from, to, f));
    network[from].push_back (edges.size() - 1);
}

void create ()
{
    network.resize(n);
    ptr.resize(n);
    d.resize(n);
}

void destroy () 
{
    edges.clear();
    for (int i = 0; i < n; ++i)
    {
        network.clear();
    }
    network.clear();
    ptr.clear();
    d.clear();
}

int main ()
{
    while (std::cin >> n) {
        if (n == 0) break;
        create();

        int m;
        std::cin >> s >> t >> m;
        s--; t--;
        for (int i = 0; i < m; ++i) {
            int a, b, f;
            std::cin >> a >> b >> f;
            a--; b--;
            
            addEdge(a, b, f);
            addEdge(b, a, 0);

            addEdge(b, a, f);
            addEdge(a, b, 0);
        }
        std::cout << dinic() << std::endl;

        destroy();
    }
    return 0;
}