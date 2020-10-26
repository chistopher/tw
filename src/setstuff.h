
#include <vector>
#include <algorithm>


using Node = int;
using NodeSet = std::vector<Node>;

/**
returns True if a is a subset of (or equal to) b
*/
bool isSubset(const NodeSet& a, const NodeSet& b) { // assumes sorted
    return std::includes(begin(b), end(b), begin(a), end(a));
}

//∪
NodeSet cup(const NodeSet& a, const NodeSet& b) {
    NodeSet res;
    set_union(begin(a), end(a), begin(b), end(b), std::back_inserter(res));
    return res;
}

// intersection (∩)
NodeSet intersection(const NodeSet& a, const NodeSet& b) {
    NodeSet res;
    set_intersection(begin(a), end(a), begin(b), end(b), std::back_inserter(res));
    return res;
}

NodeSet setminus(const NodeSet& a, const NodeSet& b) {
    NodeSet res;
    set_difference(begin(a), end(a), begin(b), end(b), back_inserter(res));
    return res;
}
