#include "LPAstar.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <queue>
#include <utility>

LPAstar::LPAstar() {
    // set defaults here
}

LPAstar::~LPAstar() {
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

SearchResult LPAstar::startSearch(const Map& map,
                                  const EnvironmentOptions& options) {
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
    while (!Open.empty()) {
        auto node = Open.begin();
        Node* curr = *node;
        if (is_g_or_equal(curr->calcKey(), goal->calcKey()) && is_equal(goal->rhs, goal->g)) {
            break;
        }
        sresult.numberofsteps++;

        OpenIterators.erase(getNodeInd(*curr, map));
        Open.erase(node);


        curr->debug = 3;
        // std::cout << "\n\n\nCURRENT: "<<curr->i << " " << curr->j << " "<<curr->g << " " << curr->rhs << " ";

        if (!is_equal(curr->g, curr->rhs) && curr->g > curr->rhs) {
            // std::cout << "g > rhs\n";
            curr->g = curr->rhs;
            for (Cell& next : map.getNeighbors(*curr, options)) {
                updateVertex(map, &nodesMap[next.i][next.j], options, false);
            }
        } else {
            // std::cout << "g <= rhs\n";
            curr->g = std::numeric_limits<double>::infinity();
            updateVertex(map, &nodesMap[curr->i][curr->j], options, false);
            for (Cell& next : map.getNeighbors(*curr, options)) {
                updateVertex(map, &nodesMap[next.i][next.j], options, false);
            }
        }
        // std::cout << "\n";
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresult.time = duration.count();
    if (goal->g != std::numeric_limits<double>::infinity()) {
        sresult.pathfound = true;
        sresult.pathlength = goal->g;
        makePrimaryPath(goal, lppath);
        makeSecondaryPath(lppath, hppath);
        sresult.hppath = &hppath;
        sresult.lppath = &lppath;
    }
    return sresult;
}

SearchResult LPAstar::repeat(const Map& map, const EnvironmentOptions& options,
                             Cell& changed) {
    Node::breakingties = options.breakingties;
    auto startTime = std::chrono::high_resolution_clock::now();
    Node* curr = &nodesMap[changed.i][changed.j];
    curr->g = std::numeric_limits<double>::infinity();
    curr->parent = nullptr;
    updateVertex(map, &nodesMap[curr->i][curr->j], options, true);

    while (!Open.empty()) {
        auto node = Open.begin();
        Node* curr = *node;
        if (is_g_or_equal(curr->calcKey(), goal->calcKey()) && is_equal(goal->rhs, goal->g)) {
            break;
        }
        sresultSecond.numberofsteps++;

        OpenIterators.erase(getNodeInd(*curr, map));
        Open.erase(node);


        curr->debug = 4;

        if (!is_equal(curr->g, curr->rhs) && curr->g > curr->rhs) {
            curr->g = curr->rhs;
            for (Cell& next : map.getNeighbors(*curr, options)) {
                updateVertex(map, &nodesMap[next.i][next.j], options, true);
            }
        } else {
            curr->g = std::numeric_limits<double>::infinity();
            updateVertex(map, &nodesMap[curr->i][curr->j], options, true);
            for (Cell& next : map.getNeighbors(*curr, options)) {
                updateVertex(map, &nodesMap[next.i][next.j], options, true);
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    sresultSecond.time = duration.count();
    if (goal->g != std::numeric_limits<double>::infinity()) {
        sresultSecond.pathfound = true;
        sresultSecond.pathlength = goal->g;
        makePrimaryPath(goal, lppathSecond);
        makeSecondaryPath(lppathSecond, hppathSecond);
        sresultSecond.hppath = &hppathSecond;
        sresultSecond.lppath = &lppathSecond;
    }
    return sresultSecond;
}

void LPAstar::updateVertex(const Map& map, Node* node, const EnvironmentOptions& options, bool isrepeat = false) {
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
        // std::cout << "UPD ("<<node->i << ", " << node->j<<")" << node->g << " "<< node->rhs << ":";
        auto it = OpenIterators.find(getNodeInd(*node, map));
        if (it != OpenIterators.end()) {
            // std::cout << "delete ";
            auto tmp = it->second;
            OpenIterators.erase(getNodeInd(**tmp, map));
            Open.erase(tmp);
            if (isrepeat) {
                --sresultSecond.nodescreated;
            } else {
                --sresult.nodescreated;
            }
        }
        if (!is_equal(node->g, node->rhs)) {
            // std::cout << "add";
            auto iterator = Open.emplace(node).first;
            OpenIterators.emplace(getNodeInd(*(*iterator), map), iterator);
            if (isrepeat) {
                ++sresultSecond.nodescreated;
            } else {
                ++sresult.nodescreated;
            }
        }
        // std::cout << "\n{";
        // for (auto it : OpenIterators) {
        //     std::cout << "("<<(*(it.second))->i << ", " << (*(it.second))->j << ")";
        // }
        // std::cout << "}\n";
        // std::cout << "[";
        // for (auto it : Open) {
        //     std::cout << "("<<it->i << ", " << it->j << ")";
        // }
        // std::cout << "]\n";
    }
}

void LPAstar::makePrimaryPath(Node* currNode, std::list<Node>& ppath1) {
    while (currNode->parent) {
        ppath1.push_front(*currNode);
        currNode = currNode->parent;
    }
    ppath1.push_front(*currNode);
}

void LPAstar::makeSecondaryPath(std::list<Node>& ppath1, std::list<Node>& ppath2) {
    int iPrev, jPrev;
    int iDiff = 0, jDiff = 0;
    Node* prevNode = &ppath1.front();
    iPrev = prevNode->i;
    jPrev = prevNode->j;
    for (auto it = std::next(ppath1.begin()); it != ppath1.end(); ++it) {
        if (it->i - iPrev != iDiff || it->j - jPrev != jDiff) {
            ppath2.push_back(*prevNode);
        }
        iDiff = it->i - iPrev;
        jDiff = it->j - jPrev;
        iPrev = it->i;
        jPrev = it->j;
        prevNode = &*it;
    }
    ppath2.push_back(ppath1.back());
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
