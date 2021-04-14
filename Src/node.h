#ifndef NODE_H
#define NODE_H

#include <utility>
#include <algorithm>
#include <cmath>

class Node {
   public:
    static unsigned int nodesCreated;
    static int breakingties;

    int i, j;                 // grid cell coordinates
    double F, g, H, rhs;           // f-, g- and h-values of the search node
    Node* parent = nullptr;   // backpointer to the predecessor node
    int debug = 0; //
    Node();
    Node(int i_, int j_, double g_);
    Node(int i_, int j_, double g_, double h_, double HW_, Node* parent_);
    Node(int i_, int j_, double g_, double h_, double HW_, Node* parent_, double rhs_);

    Node(const Node& copied);

    std::pair<double, double> calcKey() const;

    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;
    bool operator<(const Node& other) const;
};

#endif
