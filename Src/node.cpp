#include "node.h"

unsigned int Node::nodesCreated = 0;

Node::Node(int i_, int j_, double g_) {
    i = i_;
    j = j_;
    g = g_;
    ++nodesCreated;
}

Node::Node(int i_, int j_, double g_, double h_, double HW_) {
    i = i_;
    j = j_;
    g = g_;
    H = h_;
    F = g + H * HW_;
    ++nodesCreated;
}

Node::Node(const Node& copied) {
    i = copied.i;
    j = copied.j;
    g = copied.g;
    F = copied.F;
    H = copied.H;
    parent = copied.parent;
    ++nodesCreated;
}

bool Node::operator==(const Node& other) const {
    return i == other.i && j == other.j;
}

bool Node::operator!=(const Node& other) const {
    return !(*this == other);
}
