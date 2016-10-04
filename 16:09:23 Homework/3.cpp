#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <cstddef>

class Network {
private:
    class Edge;
    class EdgeIterrator;

    const std::size_t INF = 1000000000;

    std::size_t vertexCount_;
    std::size_t source_, target_;

    std::vector<Edge> edges_;
    std::vector<std::vector<std::size_t> > network_;
    std::vector<EdgeIterrator> vIter_;
    std::vector<std::size_t> distance_;

    std::size_t maxFlow_;

public:
    Network (std::size_t vertexCount, std::size_t source, std::size_t target) 
        : vertexCount_ (vertexCount), source_ (source), target_ (target)
        , maxFlow_ (0), network_ (vertexCount), distance_(vertexCount)
    {
        for (std::size_t i = 0; i < vertexCount_; ++i) {
            vIter_.push_back (begin(i));
        }
    }

    int addEdge (std::size_t from, std::size_t to, std::size_t capasity) 
    {
        std::size_t id = edges_.size();
        edges_.push_back (Edge (from, to, capasity));
        edges_.push_back (Edge (to, from, 0));

        network_[from].push_back (id);
        network_[to].push_back (id | 1);
        return id >> 1;
    }

    void addCapasity (std::size_t id, std::size_t addCapasity)
    {
        id <<= 1;
        edges_[id].addCapasity (addCapasity);
    }

    int getMaxFlow () 
    {
        return dinic();
    }

    EdgeIterrator begin(std::size_t v){
        return EdgeIterrator (network_[v].begin(), &edges_);
    }
    EdgeIterrator end (std::size_t v) {
        return EdgeIterrator (network_[v].end(), &edges_);
    }

private:
    class Edge {
    public:
        std::size_t capasity_, from_, to_;
        int flow_;
        Edge (std::size_t from, std::size_t to, std::size_t capasity) 
            : from_ (from), to_ (to), capasity_ (capasity), flow_ (0) 
        {}

        std::size_t getRest () {
            if (flow_ >= 0) {
                return capasity_ - (std::size_t) ( flow_);
            } else {
                return capasity_ + (std::size_t) (-flow_);
            }
            
        }
        bool isFull () {
            return getRest() == 0;
        }
        void addCapasity (int addCapasity)
        {
            capasity_ += addCapasity;
        }
    };

    class EdgeIterrator 
        : public std::iterator<std::random_access_iterator_tag,
                               Edge,
                               ptrdiff_t,
                               Edge*,
                               Edge&>
    {
    private:
        friend class Edge;

        std::vector<std::size_t>::iterator it_;
        std::vector<Edge> *pEdges_;
    public:

        EdgeIterrator (std::vector<std::size_t>::iterator it, std::vector<Edge> *pEdges)
        {
            it_ = it;
            pEdges_ = pEdges;
        }

        EdgeIterrator (const EdgeIterrator& edgeIterrator) = default;
        ~EdgeIterrator (){}

        EdgeIterrator& operator= (const EdgeIterrator& edgeIterator) = default;

        bool operator== (const EdgeIterrator& edgeIterator) const
        {
            return (it_ == edgeIterator.getConstPtr());
        }
        bool operator!= (const EdgeIterrator& edgeIterator) const 
        {
            return (it_ != edgeIterator.getConstPtr());
        }

        EdgeIterrator& operator+= (const ptrdiff_t& movement)
        {
            it_ += movement;
            return (*this);
        }
        EdgeIterrator& operator-= (const ptrdiff_t& movement)
        {
            it_ -= movement;
            return (*this);
        }
        EdgeIterrator& operator++ ()
        {
            ++it_;
            return (*this);
        }
        EdgeIterrator& operator-- ()
        {
            --it_;
            return (*this);
        }
        EdgeIterrator operator++ (int)
        {
            auto tmp(*this);
            ++it_;
            return tmp;
        }
        EdgeIterrator operator-- (int)
        {
            auto tmp(*this);
            --it_;
            return tmp;
        }
        EdgeIterrator operator+ (const ptrdiff_t& movement)
        {
            auto oldPtr = it_;
            it_ += movement;
            auto tmp(*this);
            it_ = oldPtr;
            return tmp;
        }
        EdgeIterrator operator- (const ptrdiff_t& movement)
        {
            auto oldPtr = it_;
            it_ -= movement;
            auto tmp(*this);
            it_ = oldPtr;
            return tmp;
        }

        ptrdiff_t operator- (const EdgeIterrator& edgeIterator)
        {
            return std::distance(edgeIterator.getPtr(),this->getPtr());
        }

        Edge& operator* () {
            return (*pEdges_)[*it_];
        }
        const Edge& operator* () const 
        {
            return (*pEdges_)[*it_];
        }
        Edge* operator-> () 
        {
            return &((*pEdges_)[*it_]);
        }
        Edge* backEdge () {
            return &((*pEdges_)[(*it_)^1]);
        }
    private:
        std::vector<std::size_t>::iterator getPtr() const
        {
            return it_;
        }
        const std::vector<std::size_t>::iterator getConstPtr() const
        {
            return it_;
        }
    };

private:
    bool dinicBfs ()
    {
        std::queue<std::size_t> queue;
        for (auto& d_i : distance_) {
            d_i = INF;
        }
        distance_[source_] = 0;
        queue.push (source_);
        while (!queue.empty() && distance_[target_] == INF) {
            int from = queue.front();
            queue.pop();

            for (auto e_i = begin(from); e_i != end(from); ++e_i) {
                if (distance_[e_i->to_] == INF && !e_i->isFull()) {
                    distance_[e_i->to_] = distance_[e_i->from_] + 1;
                    queue.push (e_i->to_);
                }
            }
        }
        return distance_[target_] != INF;
    } 

    std::size_t dinicDfs (std::size_t v, std::size_t flow)
    {
        if (flow == 0 || v == target_) return flow;
        for (; vIter_[v] != end(v); ++vIter_[v]) {
            auto e_i = vIter_[v];

            if (distance_[e_i->to_] == distance_[v] + 1) {

                std::size_t pushed = dinicDfs(e_i->to_, std::min(flow, e_i->getRest()));

                if (pushed > 0) {
                    e_i->flow_ += pushed;
                    e_i.backEdge()->flow_ -= pushed;

                    return pushed;
                }
            }
        }
        return 0;
    }

    std::size_t dinic ()
    {
        std::size_t maxFlow = 0;
        while (dinicBfs()) {
            
            for (std::size_t i = 0; i < vertexCount_; ++i) {
                vIter_[i] = begin(i);
            }
            while (std::size_t pushed = dinicDfs (source_, INF)) {
                maxFlow += pushed;
            }
        }
        return maxFlow;
    }
};

std::string s, p;
std::vector<int> compS, compP;
std::vector<std::pair<bool, int> > ret;
std::vector<std::vector<int> > ids;
std::vector<bool> used;
Network *pNetwork = nullptr;

void dfs (int v) {
    if (v != 0) {
        if (ret[v - 2].first) {
            p[ret[v - 2].second] = '0';
        } else {
            s[ret[v - 2].second] = '0';
        }
    }

    used[v] = true;
    for (auto e_i = pNetwork->begin(v); e_i != pNetwork->end(v); ++e_i) {
        if (!e_i->isFull() && !used[e_i->to_]) {
            dfs (e_i->to_);
        }
    }
}

void addEdge (int from, int to, int capasity) 
{
    int id = ids[from][to];
    if (id == -1) {
        ids[from][to] = pNetwork->addEdge (from, to, 1);
    } else {
        pNetwork->addCapasity (id, 1);
    }
}

int main ()
{
    
    std::cin >> s >> p;
    compS.resize(s.size());
    compP.resize(p.size());
    int n = 2;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '?') {
            compS[i] = n++;
            ret.push_back(std::make_pair(false, i));
        }
    }
    for (int i = 0; i < p.size(); ++i) {
        if (p[i] == '?') {
            compP[i] = n++;
            ret.push_back(std::make_pair(true, i));
        }
    }

    Network network (n, 0, 1);
    pNetwork = &network;
    ids.resize(n);
    used.resize(n, false);
    for (auto& id_i : ids) {
        id_i.resize (n, -1);
    }

    int add = 0;
    for (int i = 0; i < p.size(); ++i) {
        for (int j = i; j < s.size() - p.size() + i + 1; ++j) {
            if (p[i] == '?' || s[j] == '?') {
                if (p[i] == s[j]) {
                    addEdge (compP[i], compS[j], 1);
                    addEdge (compS[j], compP[i], 1);
                } else if (p[i] == '?') {
                    if (s[j] == '0') {
                        addEdge (0, compP[i], 1);
                    } else /* s[j] == '1' */ {
                        addEdge (compP[i], 1, 1);
                    }
                } else /* s[j] == '?' */ {
                    if (p[i] == '0') {
                        addEdge (0, compS[j], 1);
                    } else /* p[i] == '1' */ {
                        addEdge (compS[j], 1, 1);
                    }
                }
            } else {
                add += (int) (p[i] != s[j]);
            }
        }
    }
    std::cout << network.getMaxFlow() + add << std::endl;

    dfs (0);

    for (auto& s_i : s) {
        if (s_i == '?') s_i = '1';
    }
    for (auto& p_i : p) {
        if (p_i == '?') p_i = '1';
    }

    std::cout << s << "\n" << p << "\n";

    return 0;
}