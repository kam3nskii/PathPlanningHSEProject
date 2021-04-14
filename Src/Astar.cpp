#include "Astar.h"

#include <algorithm>
#include <chrono>
#include <cmath>

Astar::Astar() {
    // set defaults here
}

Astar::~Astar() {
    
}

SearchResult Astar::startSearch(const Map& map,
                                 const EnvironmentOptions& options) {
    auto startTime = std::chrono::high_resolution_clock::now();
    Node::breakingties = options.breakingties;
    auto iterator = Open.emplace(map.getStart_i(),
                                 map.getStart_j(),
                                 0,
                                 heuristic(options,
                                           map.getStart_i(),
                                           map.getStart_j(),
                                           map.getGoal_i(),
                                           map.getGoal_j()),
                                 options.hweight,
                                 nullptr)
                        .first;
    OpenIterators.emplace(getNodeInd(*iterator, map), iterator);
    ++sresult.nodescreated;
    while (!Open.empty()) {
        sresult.numberofsteps++;
        auto node = Open.begin();

        Close.emplace(getNodeInd(*node, map), *node);
        Node* curr = &Close.at(getNodeInd(*node, map));
        OpenIterators.erase(getNodeInd(*node, map));
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

        for (Cell& next : map.getNeighbors(*curr, options)) {
            if (Close.find(getNodeInd(next, map)) != Close.end()) {
                continue;
            }
            auto it = OpenIterators.find(getNodeInd(next, map));
            if (it == OpenIterators.end()) {
                auto iterator =
                    Open.emplace(
                            next.i,
                            next.j,
                            curr->g + map.getTransitionCost(curr->i, curr->j, next.i, next.j),
                            heuristic(options, next.i, next.j, map.getGoal_i(), map.getGoal_j()),
                            options.hweight,
                            curr)
                        .first;
                OpenIterators.emplace(getNodeInd(next, map), iterator);
                ++sresult.nodescreated;
            } else {
                if (it->second->g >
                    curr->g + map.getTransitionCost(curr->i, curr->j, next.i, next.j)) {
                    auto iterator =
                        Open.emplace(
                                next.i,
                                next.j,
                                curr->g + map.getTransitionCost(curr->i, curr->j, next.i, next.j),
                                heuristic(options,
                                          next.i,
                                          next.j,
                                          map.getGoal_i(),
                                          map.getGoal_j()),
                                options.hweight,
                                curr)
                            .first;
                    auto tmp = it->second;
                    OpenIterators.at(getNodeInd(*tmp, map)) = iterator;
                    Open.erase(tmp);
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

void Astar::makePrimaryPath(Node* currNode) {
    while (currNode->parent) {
        lppath.push_front(*currNode);
        currNode = currNode->parent;
    }
    lppath.push_front(*currNode);
}

void Astar::makeSecondaryPath() {
    int iPrev, jPrev;
    int iDiff = 0, jDiff = 0;
    Node* prevNode = &lppath.front();
    iPrev = prevNode->i;
    jPrev = prevNode->j;
    for (auto it = std::next(lppath.begin()); it != lppath.end(); ++it) {
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

double Astar::heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2) {
    if (options.searchtype == CN_SP_ST_DIJK) {
        return 0;
    }
    int dx = std::abs(i2 - i1);
    int dy = std::abs(j2 - j1);
    double ans;
    switch (options.metrictype) {
        case 0:   // "diagonal"
            ans = std::abs(dx - dy) + CN_SQRT_TWO * std::min(dx, dy);
            break;
        case 1:   // "manhattan" only when diagonal moves are allowed
            ans = dx + dy;
            break;
        case 2:   // "euclidean"
            ans = std::sqrt(dx * dx + dy * dy);
            break;
        case 3:   // "chebyshev"
            ans = std::max(dx, dy);
            break;
        default:
            ans = 0;
            break;
    }
    return ans;
}

int Astar::getNodeInd(const Node& node, const Map& map) {
    return node.i * map.getMapWidth() + node.j;
}

int Astar::getNodeInd(const Cell& cell, const Map& map) const {
    return cell.i * map.getMapWidth() + cell.j;
}
