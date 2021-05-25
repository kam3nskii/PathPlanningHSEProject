#ifndef DSTARLITE_H
#define DSTARLITE_H

#include <chrono>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <unordered_map>
#include <vector>

#include "cell.h"
#include "environmentoptions.h"
#include "map.h"
#include "node.h"
#include "searchresult.h"
#include "commonfuncs.h"

class DstarLite {
   public:
    DstarLite();
    ~DstarLite(void);
    void cleanup();
    void computePath(const Map& map, const EnvironmentOptions& options, int debug);
    SearchResult startSearch(const Map& Map, const EnvironmentOptions& options);
    SearchResult repeat(const Map& map, const EnvironmentOptions& options, const Cell& changed);
    SearchResult replan(const Map& map, const EnvironmentOptions& options, Node* found);
    void makePrimaryPath(Node* curNode);
    void makeSecondaryPath();
    void printSearchResult(const SearchResult& sr);
    double heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2);
    int getNodeInd(const Node& node, const Map& map);
    int getNodeInd(const Cell& cell, const Map& map) const;
    void updateVertex(const Map& map, Node* node, const EnvironmentOptions& options);

    class queue_cmp {
       public:
        bool operator()(Node* const lft, Node* const rht) const {
            auto [lftK1, lftK2] = lft->getKeysDstar();
            auto [rhtK1, rhtK2] = rht->getKeysDstar();

            if (is_equal(lftK1, rhtK1)) {
                if (is_equal(lftK2, rhtK2)) {
                    return *lft < *rht;
                } else {
                    return lftK2 < rhtK2;
                }
            } else {
                return lftK1 < rhtK1;
            }
        }
    };

    std::set<Node*, queue_cmp> Open;
    std::unordered_map<int, std::set<Node*>::iterator> OpenIterators;
    std::vector<std::vector<Node>> nodesMap;
    Node* start;
    Node* goal;
    Node* last;
    SearchResult sresult;
    std::list<Node> lppath, hppath;
    int nodesCntInPath;
    static double Km;
};
#endif
