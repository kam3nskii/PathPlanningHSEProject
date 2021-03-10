#ifndef NODE_H
#define NODE_H

class Node {
   public:
    static unsigned int nodesCreated;
    static int breakingties;

    int i, j;                 // grid cell coordinates
    double F, g, H;           // f-, g- and h-values of the search node
    Node* parent = nullptr;   // backpointer to the predecessor node

    Node(int i_, int j_, double g_);
    Node(int i_, int j_, double g_, double h_, double HW_, Node* parent_);

    Node(const Node& copied);

    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;
    bool operator<(const Node& other) const;
};

#endif
