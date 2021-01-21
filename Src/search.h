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
    //CODE HERE

    void makePrimaryPath(Node* curNode);
    void makeSecondaryPath();
    double heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2);
    int getCloseInd(const Node& node, const Map& map);

    std::list<Node> Open;
    std::unordered_map<int, Node> Close;

    //Hint 1. You definetely need class variables for OPEN and CLOSE

    //Hint 2. It's a good idea to define a heuristic calculation function, that will simply return 0
    //for non-heuristic search methods like Dijkstra

    //Hint 3. It's a good idea to define function that given a node (and other stuff needed)
    //will return it's sucessors, e.g. unordered list of nodes

    //Hint 4. working with OPEN and CLOSE is the core
    //so think of the data structures that needed to be used, about the wrap-up classes (if needed)
    //Start with very simple (and ineffective) structures like list or vector and make it work first
    //and only then begin enhancement!

    SearchResult sresult;  //This will store the search result
    std::list<Node> lppath, hppath;

    //CODE HERE to define other members of the class
};
#endif
