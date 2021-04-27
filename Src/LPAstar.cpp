#include "LPAstar.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <queue>
#include <utility>

LPAstar::LPAstar() {
}

LPAstar::~LPAstar() {
}

void LPAstar::cleanup() {
    Open.clear();
    OpenIterators.clear();
    nodesMap.clear();
    start = nullptr;
    goal = nullptr;
    sresult = SearchResult();
    lppath.clear();
    hppath.clear();
    nodesCntInPath = 0;
}

bool is_equal(double x, double y) {
    if (x == std::numeric_limits<double>::infinity() &&
        y != std::numeric_limits<double>::infinity()) {
        return false;
    }
    if (y == std::numeric_limits<double>::infinity() &&
        x != std::numeric_limits<double>::infinity()) {
        return false;
    }
    if (x == std::numeric_limits<double>::infinity() &&
        y == std::numeric_limits<double>::infinity()) {
        return true;
    }
    return std::fabs(x - y) < std::numeric_limits<double>::epsilon();
}

bool is_g_or_equal(const std::pair<double, double>& x, const std::pair<double, double>& y) {
    if (is_equal(x.first, y.first) && is_equal(x.second, y.second)) {
        return true;
    }
    return x > y;
}

void LPAstar::computePath(const Map& map, const EnvironmentOptions& options, int debug) {
    while (!Open.empty()) {
        auto node = Open.begin();
        Node* curr = *node;
        if (is_g_or_equal(curr->calcKey(), goal->calcKey()) && is_equal(goal->rhs, goal->g)) {
            break;
        }
        sresult.numberofsteps++;
        OpenIterators.erase(getNodeInd(*curr, map));
        Open.erase(node);
        curr->debug = debug;
        if (!is_equal(curr->g, curr->rhs) && curr->g > curr->rhs) {
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
    }
}

SearchResult LPAstar::startSearch(const Map& map, const EnvironmentOptions& options) {
    cleanup();
    nodesMap.resize(map.getMapHeight(), std::vector<Node>(map.getMapWidth()));
    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            Node tmp(i,
                     j,
                     std::numeric_limits<double>::infinity(),
                     heuristic(options, i, j, map.getGoal_i(), map.getGoal_j()),
                     options.hweight,
                     nullptr,
                     std::numeric_limits<double>::infinity());
            nodesMap[i][j] = tmp;
        }
    }
    sresult.nodescreated = map.getMapHeight() * map.getMapWidth();
    start = &nodesMap[map.getStart_i()][map.getStart_j()];
    goal = &nodesMap[map.getGoal_i()][map.getGoal_j()];
    start->rhs = 0;
    auto iterator = Open.emplace(start).first;
    OpenIterators.emplace(getNodeInd(**iterator, map), iterator);
    auto startTime = std::chrono::high_resolution_clock::now();
    computePath(map, options, 3);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresult.time = duration.count();
    if (goal->g != std::numeric_limits<double>::infinity()) {
        sresult.pathfound = true;
        sresult.pathlength = goal->g;
        makePrimaryPath(goal);
        makeSecondaryPath();
        sresult.hppath = &hppath;
        sresult.lppath = &lppath;
    }
    return sresult;
}

SearchResult LPAstar::repeat(const Map& map, const EnvironmentOptions& options,
                             const Cell& changed) {
    sresult = SearchResult();
    lppath.clear();
    hppath.clear();
    Node::breakingties = options.breakingties;
    auto startTime = std::chrono::high_resolution_clock::now();

    Node* tmp = &nodesMap[changed.i][changed.j];
    updateVertex(map, &nodesMap[tmp->i][tmp->j], options);
    for (Cell& next : map.getNeighbors(*tmp, options)) {
        updateVertex(map, &nodesMap[next.i][next.j], options);
    }

    computePath(map, options, 4);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresult.time = duration.count();
    if (goal->g != std::numeric_limits<double>::infinity()) {
        sresult.pathfound = true;
        sresult.pathlength = goal->g;
        makePrimaryPath(goal);
        makeSecondaryPath();
        sresult.hppath = &hppath;
        sresult.lppath = &lppath;
    }
    return sresult;
}

void LPAstar::updateVertex(const Map& map, Node* node, const EnvironmentOptions& options) {
    if (!(node->i == map.getStart_i() && node->j == map.getStart_j())) {
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
    }
    auto it = OpenIterators.find(getNodeInd(*node, map));
    if (it != OpenIterators.end()) {
        auto tmp = it->second;
        OpenIterators.erase(getNodeInd(**tmp, map));
        Open.erase(tmp);
        --sresult.nodescreated;
    }
    if (!is_equal(node->g, node->rhs)) {
        auto iterator = Open.emplace(node).first;
        OpenIterators.emplace(getNodeInd(*(*iterator), map), iterator);
        ++sresult.nodescreated;
    }
}

void LPAstar::makePrimaryPath(Node* currNode) {
    nodesCntInPath = 0;
    while (currNode->parent) {
        lppath.push_front(*currNode);
        currNode = currNode->parent;
        ++nodesCntInPath;
    }
    lppath.push_front(*currNode);
    ++nodesCntInPath;
}

void LPAstar::makeSecondaryPath() {
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

double LPAstar::heuristic(const EnvironmentOptions& options, int i1, int j1, int i2, int j2) {
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

int LPAstar::getNodeInd(const Node& node, const Map& map) {
    return node.i * map.getMapWidth() + node.j;
}

int LPAstar::getNodeInd(const Cell& cell, const Map& map) const {
    return cell.i * map.getMapWidth() + cell.j;
}
