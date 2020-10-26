
#include <bits/stdc++.h>

#include "setstuff.h"

using namespace std;

using Node = int;
using NodeSet = vector<Node>;

struct Graph {
    vector<NodeSet> adj;
    NodeSet neighs(int i) const {
        return adj[i];
    }
    NodeSet neighs(NodeSet nodes) {
        sort(begin(nodes), end(nodes));
        NodeSet res;
        for(auto v : nodes)
            res.insert(end(res), begin(adj[v]), end(adj[v]));
        sort(begin(res), end(res));
        res.erase(unique(begin(res), end(res)), end(res));
        res.erase(set_difference(begin(res), end(res), begin(nodes), end(nodes), begin(res)), end(res));
        return res;
    }
    vector<int> cneighs(int i) const { // closed neighbourhood
        auto ni = neighs(i);
        ni.push_back(i);
        return ni;
    }
};

struct Component { 
    NodeSet nodes;
    bool operator<(const Component& other) const {
        return nodes < other.nodes;
    }
};
struct IBlock : Component { };
struct OBlock : Component { };

struct PMC {
    PMC(const NodeSet& nodes_) : nodes(nodes_) {

    };

    NodeSet nodes;
    NodeSet outlet() const {
        // TODO
        return {};
    };
};

bool isFeasible(const PMC&) { return true; }; // TODO
bool isCliquish(const NodeSet&) { return true; }; // TODO
vector<Component> fullComponents(const NodeSet&) { return {}; }; // TODO
bool isPMC(const NodeSet& nodes) {
    return isCliquish(nodes) && fullComponents(nodes).size()==0;
}
NodeSet crib(const PMC&){ // TODO; returns crib(outlet(PMC), PMC)
    return NodeSet();
}


int main() {

    Graph g;
    int n = (int)size(g.adj);
    unsigned int targetWidth = 2;


    vector<PMC> buildablepmcs; // for debug; they may be feasible now?

    set<IBlock> iblocks;
    set<OBlock> oblocks;
    vector<PMC> pmcs;

    queue<IBlock> pendingIBlocks;

    auto processPMC = [&](const PMC& pmc) {
        if(!isFeasible(pmc)) {
            buildablepmcs.push_back(pmc);
            return;
        }
        if(size(pmc.outlet())==0) {
            cout << "YAY!" << endl;
            exit(0);
        }
        pmcs.push_back(pmc);

        IBlock maybeNewIblock = IBlock{crib(pmc)};
        if(!iblocks.count(maybeNewIblock)) {
            iblocks.insert(maybeNewIblock);
            pendingIBlocks.push(move(maybeNewIblock));
        }
            
    };

    // find all case 1 buildable PMCs (N[v])
    for(int i=0; i<n; ++i) {
        auto ni = g.neighs(i);
        if(isPMC(ni)) {
            PMC foundPMC{ni};
            processPMC(foundPMC);
        }
    }

    while (!pendingIBlocks.empty()) {
        auto currentI = pendingIBlocks.front();
        pendingIBlocks.pop();

        vector<OBlock> newOblocks;
        
        for(const OBlock& oblock : oblocks) {
            /* 
                if (!currentI ⊂ oblock)
                    continue
                K = N(C) ∪ N(B)
                if (|K| <= k+1 && isPMC(k)) {
                    --> yay (maybe new) pmc
                } else if (|K| <= k && fullComponents(K).size()==1) {
                    maybeNewOblock = fullComponents(K)[0]
                    --> yay (maybe new) oblock
                }
            */

            if(!isSubset(currentI.nodes, oblock.nodes))
                continue;

            auto K = cup(
                g.neighs(currentI.nodes),
                g.neighs(oblock.nodes)
            );
        
            if(size(K) <= targetWidth+1 && isPMC(K)) {
                // --> yay (maybe new) pmc
                processPMC({K});
                
            } else if (size(K) <= targetWidth && fullComponents(K).size()==1) {
                // --> yay (maybe new) oblock found
                OBlock maybeNewOblock{fullComponents(K).front()};
                if(oblocks.count(maybeNewOblock)==0) // vielleicht ist er auch immer neu?
                    newOblocks.push_back(maybeNewOblock);
            }

        
        }

        /*
        search outbound component A with N(A) == N(currentI)
        --> yay (maybe new) oblock
        */
        smallest = infty
        outBound = None
        for fullComp fullComponents(g.neighs(currentI)) {
            if minNode(fullComp) < smallest:
                outBound = fullComponents
                smallest = minNode(fullComp)
        }

        /*
        for (Oblock oblock : newOblocks) {
            for (Node v: neighs(oblock)) {
                K = neighs(oblock) ∪ (neighs(v) ∩ oblock)
                if (|K| <= k+1 && isPMC(K))
                    --> yay (maybe new) PMC found
            }
        }

        */


    }


    return 0;
}

