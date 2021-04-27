#ifndef LPASTAR_H
#define LPASTAR_H

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

bool is_equal(double x, double y);

bool is_g_or_equal(const std::pair<double, double>& x, const std::pair<double, double>& y);

class LPAstar {
   public:
    LPAstar();
    ~LPAstar(void);
    void cleanup();
    void computePath(const Map& map, const EnvironmentOptions& options, int debug);
    SearchResult startSearch(const Map& Map, const EnvironmentOptions& options);
    SearchResult repeat(const Map& map, const EnvironmentOptions& options, const Cell& changed);
    void makePrimaryPath(Node* curNode);
    void makeSecondaryPath();
    double heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2);
    int getNodeInd(const Node& node, const Map& map);
    int getNodeInd(const Cell& cell, const Map& map) const;
    void updateVertex(const Map& map, Node* node, const EnvironmentOptions& options);

    class queue_cmp {
       public:
        bool operator()(Node* const lft, Node* const rht) const {
            auto [lftK1, lftK2] = lft->calcKey();
            auto [rhtK1, rhtK2] = rht->calcKey();

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
    SearchResult sresult;
    std::list<Node> lppath, hppath;
    int nodesCntInPath;
};
#endif
