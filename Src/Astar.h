#ifndef ASTAR_H
#define ASTAR_H

#include <chrono>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <unordered_map>
#include <vector>

#include "cell.h"
#include "environmentoptions.h"
#include "gl_const.h"
#include "map.h"
#include "node.h"
#include "searchresult.h"
#include "tinyxml2.h"

class Astar {
   public:
    Astar();
    ~Astar(void);
    SearchResult startSearch(const Map& Map, const EnvironmentOptions& options);

    //    protected:
    void makePrimaryPath(Node* curNode);
    void makeSecondaryPath();
    double heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2);
    int getNodeInd(const Node& node, const Map& map);
    int getNodeInd(const Cell& cell, const Map& map) const;

    std::set<Node> Open;
    std::unordered_map<int, std::set<Node>::iterator> OpenIterators;
    std::unordered_map<int, Node> Close;

    SearchResult sresult;
    std::list<Node> lppath, hppath;
};
#endif
