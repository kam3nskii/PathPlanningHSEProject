#include "DstarLite.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <queue>
#include <utility>

DstarLite::DstarLite() {
}

DstarLite::~DstarLite() {
}

double DstarLite::Km = 0;

void DstarLite::cleanup() {
    Open.clear();
    OpenIterators.clear();
    nodesMap.clear();
    start = nullptr;
    goal = nullptr;
    sresult = SearchResult();
    lppath.clear();
    hppath.clear();
    nodesCntInPath = 0;
    Km = 0;
}

void DstarLite::computePath(const Map& map, const EnvironmentOptions& options, int debug) {
    while (!Open.empty()) {
        auto node = Open.begin();
        Node* curr = *node;
        if (isLess(curr->getKeysDstar(), start->calcKeyDstar(Km)) ||
            !is_equal(start->rhs, start->g)) {
            sresult.numberofsteps++;
            OpenIterators.erase(getNodeInd(*curr, map));
            Open.erase(node);
            curr->debug = debug;
            auto Kold = curr->getKeysDstar();

            if (isLess(Kold, curr->calcKeyDstar(Km))) {
                curr->setKeysDstar(Km);
                auto iterator = Open.emplace(curr).first;
                OpenIterators.emplace(getNodeInd(*(*iterator), map), iterator);
                ++sresult.nodescreated;
            } else if (!is_equal(curr->g, curr->rhs) && curr->g > curr->rhs) {
                curr->g = curr->rhs;
                for (Cell& next : map.getNeighbors(*curr, options)) {
                    updateVertex(map, &nodesMap[next.i][next.j], options);
                }
            } else {
                curr->g = std::numeric_limits<double>::infinity();
                updateVertex(map, &nodesMap[curr->i][curr->j], options);
                for (Cell& next : map.getNeighbors(*curr, options)) {
                    updateVertex(map, &nodesMap[next.i][next.j], options);
                }
            }
        } else {
            break;
        }
    }
}

SearchResult DstarLite::startSearch(const Map& map, const EnvironmentOptions& options) {
    cleanup();
    nodesMap.resize(map.getMapHeight(), std::vector<Node>(map.getMapWidth()));
    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            Node tmp(i,
                     j,
                     std::numeric_limits<double>::infinity(),
                     heuristic(options, map.getStart_i(), map.getStart_j(), i, j),
                     options.hweight,
                     nullptr,
                     std::numeric_limits<double>::infinity());
            nodesMap[i][j] = tmp;
        }
    }
    sresult.nodescreated = map.getMapHeight() * map.getMapWidth();
    start = &nodesMap[map.getStart_i()][map.getStart_j()];
    last = &nodesMap[map.getStart_i()][map.getStart_j()];
    goal = &nodesMap[map.getGoal_i()][map.getGoal_j()];
    goal->rhs = 0;
    goal->setKeysDstar(Km);
    auto iterator = Open.emplace(goal).first;
    OpenIterators.emplace(getNodeInd(**iterator, map), iterator);
    auto startTime = std::chrono::high_resolution_clock::now();
    computePath(map, options, 3);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresult.time = duration.count();
    if (start->g != std::numeric_limits<double>::infinity()) {
        sresult.pathfound = true;
        sresult.pathlength = start->g;
        makePrimaryPath(start);
        makeSecondaryPath();
        sresult.hppath = &hppath;
        sresult.lppath = &lppath;
    }
    return sresult;
}

SearchResult DstarLite::repeat(const Map& map, const EnvironmentOptions& options,
                               const Cell& changed) {
    sresult = SearchResult();
    lppath.clear();
    hppath.clear();
    Node::breakingties = options.breakingties;
    auto startTime = std::chrono::high_resolution_clock::now();

    Node* tmp = &nodesMap[changed.i][changed.j];
    nodesMap[tmp->i][tmp->j].g = std::numeric_limits<double>::infinity();
    nodesMap[tmp->i][tmp->j].rhs = std::numeric_limits<double>::infinity();
    nodesMap[tmp->i][tmp->j].parent = nullptr;
    for (Cell& next : map.getNeighbors(*tmp, options)) {
        updateVertex(map, &nodesMap[next.i][next.j], options);
    }
    computePath(map, options, 4);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresult.time = duration.count();
    if (start->g != std::numeric_limits<double>::infinity()) {
        sresult.pathfound = true;
        sresult.pathlength = start->g;
        makePrimaryPath(start);
        makeSecondaryPath();
        sresult.hppath = &hppath;
        sresult.lppath = &lppath;
    }
    return sresult;
}

void DstarLite::printSearchResult(const SearchResult& sr) {
    std::cout << "Path ";
    if (!sr.pathfound) {
        std::cout << "NOT ";
    }
    std::cout << "found!" << std::endl;
    std::cout << "numberofsteps=" << sr.numberofsteps << std::endl;
    std::cout << "nodescreated=" << sr.nodescreated << std::endl;
    if (sr.pathfound) {
        std::cout.setf(std::ios::fixed);
        std::cout.precision(5);
        std::cout << "pathlength=" << sr.pathlength << std::endl;
        // std::cout << "pathlength_scaled=" << sr.pathlength * map.getCellSize() << std::endl;
    }
    std::cout.precision(15);
    std::cout << "time=" << sr.time << std::endl;
}

SearchResult DstarLite::replan(const Map& map, const EnvironmentOptions& options, Node* found) {
    Km = Km + heuristic(options, last->i, last->j, start->i, start->j);
    last = start;
    sresult = SearchResult();
    lppath.clear();
    hppath.clear();

    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            nodesMap[i][j].H = options.hweight * heuristic(options, start->i, start->j, i, j);
        }
    }

    Node::breakingties = options.breakingties;
    auto startTime = std::chrono::high_resolution_clock::now();

    for (Cell& next : map.getNeighbors(*found, options)) {
        updateVertex(map, &nodesMap[next.i][next.j], options);
    }
    computePath(map, options, 4);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresult.time = duration.count();
    if (start->g != std::numeric_limits<double>::infinity()) {
        sresult.pathfound = true;
        sresult.pathlength = start->g;
        makePrimaryPath(start);
        makeSecondaryPath();
        sresult.hppath = &hppath;
        sresult.lppath = &lppath;
    } else {
        sresult.pathfound = false;
    }

    return sresult;
}

void DstarLite::updateVertex(const Map& map, Node* node, const EnvironmentOptions& options) {
    if (!(node->i == map.getGoal_i() && node->j == map.getGoal_j())) {
        node->rhs = std::numeric_limits<double>::infinity();
        node->parent = nullptr;
        for (Cell& next : map.getNeighbors(*node, options)) {
            Node* tmp = &nodesMap[next.i][next.j];
            double res = tmp->g + map.getTransitionCost(node->i, node->j, tmp->i, tmp->j);
            if (res < node->rhs) {
                node->rhs = res;
                node->parent = tmp;
            }
        }
        auto it = OpenIterators.find(getNodeInd(*node, map));
        if (it != OpenIterators.end()) {
            auto tmp = it->second;
            OpenIterators.erase(getNodeInd(**tmp, map));
            Open.erase(tmp);
            --sresult.nodescreated;
        }
        if (!is_equal(node->g, node->rhs)) {
            node->setKeysDstar(Km);
            auto iterator = Open.emplace(node).first;
            OpenIterators.emplace(getNodeInd(*(*iterator), map), iterator);
            ++sresult.nodescreated;
        }
    }
}

void DstarLite::makePrimaryPath(Node* currNode) {
    nodesCntInPath = 0;
    while (currNode->parent) {
        lppath.push_front(*currNode);
        currNode = currNode->parent;
        ++nodesCntInPath;
    }
    lppath.push_front(*currNode);
    ++nodesCntInPath;
}

void DstarLite::makeSecondaryPath() {
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

double DstarLite::heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2) {
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

int DstarLite::getNodeInd(const Node& node, const Map& map) {
    return node.i * map.getMapWidth() + node.j;
}

int DstarLite::getNodeInd(const Cell& cell, const Map& map) const {
    return cell.i * map.getMapWidth() + cell.j;
}
