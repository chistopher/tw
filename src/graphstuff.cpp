
#include "graphstuff.h"

#include <algorithm>
#include <queue>
#include <map>
#include <cassert>

using namespace std;

bool Graph::adjacent(Node a, Node b) const {
    return binary_search(begin(adj[a]), end(adj[a]), b); // TODO benchmark if find is faster
}

NodeSet Graph::neighs(int i) const {
    assert(is_sorted(begin(adj[i]), end(adj[i])));
    return adj[i];
}

NodeSet Graph::neighs(NodeSet nodes) const {
    assert(is_sorted(begin(nodes), end(nodes)));
    // sort(begin(nodes), end(nodes));
    NodeSet res;
    for(auto v : nodes)
        res.insert(end(res), begin(adj[v]), end(adj[v]));
    sort(begin(res), end(res));
    res.erase(unique(begin(res), end(res)), end(res));
    res.erase(set_difference(begin(res), end(res), begin(nodes), end(nodes), begin(res)), end(res));
    return res;
}

NodeSet Graph::cneighs(int i) const { // closed neighbourhood
    auto ni = neighs(i);
    ni.insert(lower_bound(begin(ni), end(ni), i), i);
    return ni;
}

bool Graph::isClique(const NodeSet& nodes) const {
    for (Node a: nodes) {
        for (Node b: nodes) {
            if (a==b)
                continue;
            if (!adjacent(a, b))
                return false;
        }
    }
    return true;
}

bool Graph::isCliquish(const NodeSet& nodes) const {
    map<int,int> idx;
    for(int i=0; i<(int)size(nodes); ++i)
        idx[nodes[i]] = i;

    vector node_adj(size(nodes), vector(size(nodes), false));

    for (auto comp: components(nodes)) {
        auto comp_neighs = neighs(comp.nodes);
        for (Node a: comp_neighs) {
            for (Node b: comp_neighs) {
                if (a >= b)
                    continue;
                assert(idx.find(a) != idx.end());
                assert(idx.find(b) != idx.end());
                node_adj[idx[a]][idx[b]] = 1;
            }
        }
    }

    for (Node a: nodes) {
        for (Node b: nodes) {
            if (a >= b)
                continue;
            if (node_adj[idx[a]][idx[b]] == 0) {
                if (!adjacent(a,b))
                    return false;
            }
        }
    }

    return true;
}


vector<int> compMap(const Graph& g, const NodeSet& separator) {
    vector<int> comps(size(g.adj), -1);
    for(auto v : separator) 
        comps[v] = 0;
    int numComps = 1;
    for(int v=0; v<(int)size(g.adj); ++v) {
        if(comps[v]!=-1) 
            continue;
        queue<int> q{{v}};
        comps[v] = numComps;
        while(size(q)) {
            auto u = q.front(); q.pop();
            for(auto nei : g.adj[u])
                if(comps[nei]==-1)
                    comps[nei] = numComps, q.push(nei);
        }
        numComps++;
    }
    return comps;
}

vector<Component> Graph::components(const NodeSet& separator) const {
    assert(is_sorted(begin(separator), end(separator)));
    auto comp = compMap(*this, separator);
    auto numComps = *max_element(begin(comp), end(comp));
    vector<Component> res(numComps);
    for(int v=0; v<(int)size(adj); ++v) {
        if(comp[v]==0) continue; // in separator
        assert(comp[v]>0);
        res[comp[v]-1].nodes.push_back(v);
    }
    return res;
}

vector<Component> Graph::fullComponents(const NodeSet& separator) const {
    assert(is_sorted(begin(separator), end(separator)));
    vector<Component> res;
    for(const Component& comp : components(separator)) {
        if(size(separator) == size(neighs(comp.nodes)))
            res.push_back(comp);
    }
    return res;
}

Graph Graph::largeCC() const {
    auto comp = compMap(*this, {});
    auto numComps = *max_element(begin(comp), end(comp));
    vector<int> compSize(numComps, 0);
    for(auto c : comp) compSize[c-1]++;
    auto largestComp = max_element(begin(compSize), end(compSize)) - begin(compSize) + 1;
    map<int,int> idMap;
    for(auto i=0u; i<size(adj); ++i)
        if(comp[i]==largestComp)
            idMap[i] = (int)size(idMap);
    assert((int)size(idMap) == compSize[largestComp-1]);

    Graph res;
    res.adj.resize(size(idMap));
    for(auto i=0u; i<size(adj); ++i) {
        if(comp[i]!=largestComp)
            continue;
        for(int nei : adj[i]) {
            assert(comp[nei]==largestComp); 
            res.adj[idMap[i]].push_back(idMap[nei]);
        }
    }

    return res;
}

bool Graph::isConnected(const NodeSet& nodes) const {
    vector<bool> visited(size(adj),true);
    for(auto v : nodes) visited[v] = false;
    queue<int> q{{nodes.front()}};
    visited[nodes.front()] = true;
    while(size(q)) {
        int v = q.front(); q.pop();
        for(auto nei : adj[v])
            if(!visited[nei]) 
                visited[nei] = true, q.push(nei);
    }
    for(auto v : nodes)
        if(!visited[v]) 
            return false;
    return true;
}