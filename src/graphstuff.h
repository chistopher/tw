
#include <vector>

using Node = int;
using NodeSet = std::vector<Node>;

struct Component { 
    NodeSet nodes; // should be sorted
    bool operator<(const Component& other) const {
        return nodes < other.nodes;
    }
};

struct Graph {
    std::vector<NodeSet> adj;

    bool adjacent(Node a, Node b) const;

    NodeSet neighs(int i) const;
    NodeSet neighs(NodeSet nodes) const;
    NodeSet cneighs(int i) const;

    bool isClique(const NodeSet&) const;
    bool isCliquish(const NodeSet&) const;

    std::vector<Component> components(const NodeSet&) const;
    std::vector<Component> fullComponents(const NodeSet&) const;

    Graph largeCC() const;
    bool isConnected(const NodeSet&) const;
};

