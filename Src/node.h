#ifndef NODE_H
#define NODE_H

//That's the data structure for storing a single search node.
//You MUST store all the intermediate computations occuring during the search
//incapsulated to Nodes (so NO separate arrays of g-values etc.)

class Node {
public:
    static unsigned int nodesCreated;

    int i, j;                //grid cell coordinates
    double F, g, H;          //f-, g- and h-values of the search node
    Node* parent = nullptr;  //backpointer to the predecessor node (e.g. the node which g-value was used to set the g-velue of the current node)

    Node(int i_, int j_, double g_);
    Node(int i_, int j_, double g_, double h_, double HW_);

    Node(const Node& copied);

    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;
};

#endif
