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
    double F, g, H, rhs;      // f-, g- and h-values of the search node
    Node* parent = nullptr;   // backpointer to the predecessor node
    double key1, key2;
    int debug = 0;
    Node();
    Node(int i_, int j_, double g_);
    Node(int i_, int j_, double g_, double h_, double HW_, Node* parent_);
    Node(int i_, int j_, double g_, double h_, double HW_, Node* parent_, double rhs_);

    std::pair<double, double> calcKeyLPA() const;
    void setKeysLPA();
    std::pair<double, double> getKeysLPA() const;

    std::pair<double, double> calcKeyDstar(double Km) const;
    void setKeysDstar(double Km);
    std::pair<double, double> getKeysDstar() const;

    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;
    bool operator<(const Node& other) const;
};

#endif
