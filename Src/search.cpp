#include "search.h"

#include <algorithm>
#include <chrono>

Search::Search() {
    //set defaults here
}

Search::~Search() {}

SearchResult Search::startSearch(ILogger* Logger, const Map& map, const EnvironmentOptions& options) {
    auto startTime = std::chrono::high_resolution_clock::now();
    Open.emplace_back(map.getStart_i(), map.getStart_j(), 0);
    while (!Open.empty()) {
        sresult.numberofsteps++;
        std::list<Node>::iterator node = Open.end();
        for (std::list<Node>::iterator it = Open.begin(); it != Open.end(); ++it) {
            if (node == Open.end() || node->g > it->g) {
                node = it;
            }
        }
        Close.emplace_back(*node);
        Node* curr = &Close.back();
        Open.erase(node);
        if (curr->i == map.getGoal_i() && curr->j == map.getGoal_j()) {
            Node* some = curr;
            while (some) {
                lppath.push_front(*some);
                some = some->parent;
            }
            int iPrev, jPrev;
            int iDiff = 0, jDiff = 0;
            Node* prevNode = curr;
            iPrev = prevNode->i;
            jPrev = prevNode->j;
            some = prevNode->parent;
            hppath.push_front(*prevNode);
            while (some) {
                if (some->i - iPrev != iDiff && some->j - jPrev != jDiff) {
                    hppath.push_front(*prevNode);
                }
                iDiff = some->i - iPrev;
                jDiff = some->j - jPrev;
                iPrev = some->i;
                jPrev = some->j;
                prevNode = some;
                some = some->parent;
            }
            hppath.push_front(*prevNode);
            sresult.pathfound = true;
            sresult.pathlength = curr->g;
            sresult.hppath = &hppath;
            sresult.lppath = &lppath;
            break;
        }
        for (Node& next : map.getNeighbors(*curr)) {
            if (map.getValue(next.i, next.j) == CN_GC_NOOBS) {
                std::list<Node>::iterator it = std::find(std::begin(Close), std::end(Close), next);
                if (it != Close.end()) {
                    continue;
                }
                it = std::find(std::begin(Open), std::end(Open), next);
                if (it == Open.end()) {
                    next.g = curr->g + 1;
                    next.parent = curr;
                    Open.push_back(next);
                } else {
                    it->g = std::min(it->g, curr->g + 1);
                    if (it->g == curr->g + 1) {
                        it->parent = curr;
                    }
                }
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresult.nodescreated = Node::nodesCreated;
    sresult.time = duration.count();
    return sresult;
}

/*void Search::makePrimaryPath(Node curNode)
{
    //need to implement
}*/

/*void Search::makeSecondaryPath()
{
    //need to implement
}*/
