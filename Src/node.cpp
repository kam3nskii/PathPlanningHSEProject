#include "node.h"

unsigned int Node::nodesCreated = 0;
int Node::breakingties = 0;

Node::Node() {
}

Node::Node(int i_, int j_, double g_) {
    i = i_;
    j = j_;
    g = g_;
    ++nodesCreated;
}

Node::Node(int i_, int j_, double g_, double h_, double HW_, Node* parent_) {
    i = i_;
    j = j_;
    g = g_;
    H = h_ * HW_;
    F = g + H;
    parent = parent_;
    ++nodesCreated;
}

Node::Node(int i_, int j_, double g_, double h_, double HW_, Node* parent_, double rhs_) {
    i = i_;
    j = j_;
    g = g_;
    H = h_ * HW_;
    F = g + H;
    parent = parent_;
    rhs = rhs_;
    ++nodesCreated;
}

Node::Node(const Node& copied) {
    i = copied.i;
    j = copied.j;
    g = copied.g;
    F = copied.F;
    H = copied.H;
    rhs = copied.rhs;
    parent = copied.parent;
    ++nodesCreated;
}

std::pair<double, double> Node::calcKey() const {
    return std::make_pair(std::min(g, rhs) + H, std::min(g, rhs));
}

bool Node::operator==(const Node& other) const {
    return i == other.i && j == other.j;
}

bool Node::operator!=(const Node& other) const {
    return !(*this == other);
}

bool Node::operator<(const Node& other) const {
    if (F < other.F) {
        return true;
    } else if (F == other.F) {
        if ((breakingties && (g > other.g)) || (!breakingties && (g < other.g))) {
            return true;
        } else if (g == other.g) {
            if (i < other.i) {
                return true;
            } else if (i == other.i) {
                if (j < other.j) {
                    return true;
                }
            }
        }
    }
    return false;
}
