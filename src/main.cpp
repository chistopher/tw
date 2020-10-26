
#include <bits/stdc++.h>

#include "setstuff.h"
#include "graphstuff.h"

using namespace std;

struct IBlock : Component { };
struct OBlock : Component { };

bool isInbound(const Graph& g, const Component& comp) {
    auto sep = g.neighs(comp.nodes);
    auto otherC = g.fullComponents(sep);
    return comp.nodes != min_element(begin(otherC), end(otherC))->nodes;
}

struct PMC {
    NodeSet nodes;
    const Graph& g;

    PMC(const NodeSet& nodes_, const Graph& g_) : nodes(nodes_), g(g_) { };

    NodeSet outlet() const {
        NodeSet res;
        for (auto comp: g.components(nodes)) {
            if (!isInbound(g,comp)) {
                auto temp = g.neighs(comp.nodes);
                if(size(temp)>size(res)) res = temp;
            }
        }
        return res;
    };

    vector<Component> support() const { // returns IBlock
        vector<Component> res;
        auto olet = outlet();
        for (const auto& comp: g.components(nodes)) {
            if(!isSubset(comp.nodes,olet))
                res.push_back(comp);
        }
        return res;
    }
};

bool isFeasible(const PMC & omega, const set<IBlock>& knownIblocks) {
    for (auto comp: omega.support()) {
        if (knownIblocks.count({comp}) == 0)
            return false;
    }
    return true;
}

bool isPMC(const Graph& g, const NodeSet& nodes) {
    assert(is_sorted(begin(nodes), end(nodes)));
    return g.isCliquish(nodes) && g.fullComponents(nodes).size()==0;
}

NodeSet crib(const PMC& omega){
    NodeSet res = setminus(omega.nodes, omega.outlet());
    for (auto comp: omega.support()) {
        res = cup(res, comp.nodes);
    }
    return res;
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
        if(!isFeasible(pmc, iblocks)) {
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
        auto ni = g.cneighs(i);
        if(isPMC(g,ni)) {
            PMC foundPMC(ni,g);
            processPMC(foundPMC);
        }
    }

    while (!pendingIBlocks.empty()) {
        auto currentI = pendingIBlocks.front();
        pendingIBlocks.pop();

        vector<OBlock> newOblocks;
        
        // case 2 PMCs
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
        
            if(size(K) <= targetWidth+1 && isPMC(g,K)) {
                // --> yay (maybe new) pmc
                processPMC({K,g});
                
            } else if (size(K) <= targetWidth && g.fullComponents(K).size()==1) {
                // --> yay (maybe new) oblock found
                OBlock maybeNewOblock{g.fullComponents(K).front()};
                if(oblocks.count(maybeNewOblock)==0) // vielleicht ist er auch immer neu?
                    newOblocks.push_back(maybeNewOblock);
                else
                    cout << "OBLOCK is not new. Our question was answered" << endl;
            }

        
        }

        /*
        search outbound component A with N(A) == N(currentI)
        --> yay (maybe new) oblock
         */
    
        auto otherComps = g.fullComponents(g.neighs(currentI.nodes));
        assert(otherComps.size()>1);
        auto otherOBlock = *min_element(begin(otherComps), end(otherComps), [](const Component& a, const Component& b) { 
            return a.nodes.front() < b.nodes.front();
        });
        assert(otherOBlock < currentI);
        if(oblocks.count({otherOBlock})==0)
            newOblocks.push_back({otherOBlock});
        else
            cout << "apparently this can be a known oblock..." << endl;

        // check if we can find oblocks multiple times
        if(size(newOblocks)>set(newOblocks.begin(), newOblocks.end()).size())
            cout << "we found some oblock multiple times" << endl;
        

        // type 3 PMCs
        // Subsequently for each O-block (N (A), A) that had already been discovered before 
        // processing the current I-block and for each v ∈ N (A),
        // all PMCs K with |K | ≤ k + 1 of the form K = N (A) ∪ (N (v) ∩ A) are registered.
        for (const OBlock& oblock : newOblocks) {
            auto neigh = g.neighs(oblock.nodes);
            for (Node v: neigh) {
                auto K = cup(neigh, intersection(oblock.nodes, g.neighs(v)));
                if (size(K) <= targetWidth+1 && isPMC(g,K)) {
                    processPMC({K,g});
                }
            }
        }

        for(const auto& oblock : newOblocks) {
            assert(oblocks.count(oblock)==0);
            oblocks.insert(oblock);
        }
    }

    // TODO check if we missed some buildable PMCs that only got feasible later

    return 0;
}

