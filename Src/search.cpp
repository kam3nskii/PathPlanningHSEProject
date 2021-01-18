#include "search.h"

#include <algorithm>
#include <chrono>
#include <cmath>

Search::Search() {
    //set defaults here
}

Search::~Search() {}

SearchResult Search::startSearch(ILogger* Logger, const Map& map, const EnvironmentOptions& options) {
    auto startTime = std::chrono::high_resolution_clock::now();
    Open.emplace_back(map.getStart_i(), map.getStart_j(), 0,
                      heuristic(options.metrictype, map.getStart_i(),
                                map.getStart_j(), map.getGoal_i(), map.getGoal_j()), options.hweight);
    ++sresult.nodescreated;
    while (!Open.empty()) {
        sresult.numberofsteps++;

        std::list<Node>::iterator node = Open.end();
        for (std::list<Node>::iterator it = Open.begin(); it != Open.end(); ++it) {
            if (options.breakingties) { // gmax
                if (node == Open.end() || node->F > it->F || (node->F == it->F && node->g < it->g)) {
                    node = it;
                }
            } else { // gmin
                if (node == Open.end() || node->F > it->F || (node->F == it->F && node->g > it->g)) {
                    node = it;
                }
            }
        }
        Close.emplace_back(*node);
        Node* curr = &Close.back();
        Open.erase(node);

        if (curr->i == map.getGoal_i() && curr->j == map.getGoal_j()) {
            makePrimaryPath(curr);
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = endTime - startTime;
            sresult.time = duration.count();
            makeSecondaryPath();
            sresult.pathfound = true;
            sresult.pathlength = curr->g;
            sresult.hppath = &hppath;
            sresult.lppath = &lppath;
            break;
        }

        for (Node& next : map.getNeighbors(*curr, options)) {
            if (std::find(std::begin(Close), std::end(Close), next) != Close.end()) {
                continue;
            }
            std::list<Node>::iterator it = std::find(std::begin(Open), std::end(Open), next);
            if (it == Open.end()) {
                next.g = curr->g + map.getTransitionCost(curr->i, curr->j, next.i, next.j);
                next.parent = curr;
                next.H = heuristic(options.metrictype, next.i, next.j, map.getGoal_i(), map.getGoal_j());
                next.F = next.g + next.H * options.hweight;
                Open.push_back(next);
                ++sresult.nodescreated;
            } else {
                it->g = std::min(it->g, curr->g + map.getTransitionCost(curr->i, curr->j, next.i, next.j));
                if (it->g == curr->g + map.getTransitionCost(curr->i, curr->j, next.i, next.j)) {
                    it->parent = curr;
                }
            }
        }
    }
    if (!sresult.pathfound) {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        sresult.time = duration.count();
    }
    return sresult;
}

void Search::makePrimaryPath(Node* currNode) {
    while (currNode->parent) {
        lppath.push_front(*currNode);
        currNode = currNode->parent;
    }
    lppath.push_front(*currNode);
}

void Search::makeSecondaryPath() {
    int iPrev, jPrev;
    int iDiff = 0, jDiff = 0;
    Node* prevNode = &lppath.front();
    iPrev = prevNode->i;
    jPrev = prevNode->j;
    for (std::list<Node>::iterator it = std::next(lppath.begin()); it != lppath.end(); ++it) {
        if (it->i - iPrev != iDiff || it->j - jPrev != jDiff) {
            hppath.push_back(*prevNode);
        }
        iDiff = it->i - iPrev;
        jDiff = it->j - jPrev;
        iPrev = it->i;
        jPrev = it->j;
        prevNode = &*it;
    }
    hppath.push_back(lppath.back());
}

double Search::heuristic(int metricType, int i1, int j1, int i2, int j2) {
    int dx = std::abs(i2 - i1);
    int dy = std::abs(j2 - j1);
    switch (metricType) {
        case 0:  // "diagonal"
            return std::abs(dx - dy) + CN_SQRT_TWO * std::min(dx, dy);
            break;
        case 1:  // "manhattan" only when diagonal moves are allowed
            return dx + dy;
            break;
        case 2:  // "euclidean"
            return std::sqrt(dx * dx + dy * dy);
            break;
        case 3:  // "chebyshev"
            return std::max(dx, dy);
            break;
        default:
            break;
    }
}
