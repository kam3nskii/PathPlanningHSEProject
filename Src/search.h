#ifndef SEARCH_H
#define SEARCH_H

#include <chrono>
#include <cmath>
#include <limits>
#include <list>
#include <unordered_map>
#include <vector>

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

    std::list<Node> Open;
    std::unordered_map<int, Node> Close;
    std::unordered_map<int, std::list<Node>::iterator> OpenIterators;

    SearchResult sresult;  //This will store the search result
    std::list<Node> lppath, hppath;
};
#endif
