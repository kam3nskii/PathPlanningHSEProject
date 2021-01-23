#ifndef SEARCH_H
#define SEARCH_H

#include <chrono>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <unordered_map>
#include <vector>

#include "cell.h"
#include "environmentoptions.h"
#include "ilogger.h"
#include "node.h"
#include "searchresult.h"

class Search {
public:
    Search();
    ~Search(void);
    SearchResult startSearch(ILogger* Logger, const Map& Map, const EnvironmentOptions& options);

protected:
    void makePrimaryPath(Node* curNode);
    void makeSecondaryPath();
    double heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2);
    int getNodeInd(const Node& node, const Map& map);
    int getNodeInd(const Cell& cell, const Map& map);

    std::set<Node> Open;
    std::unordered_map<int, std::set<Node>::iterator> OpenIterators;
    std::unordered_map<int, Node> Close;

    SearchResult sresult;  //This will store the search result
    std::list<Node> lppath, hppath;
};
#endif
