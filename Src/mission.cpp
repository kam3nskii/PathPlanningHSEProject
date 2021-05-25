#include "mission.h"

#include <iomanip>
#include <iostream>

#include "cell.h"

Mission::Mission() {
    fileName = nullptr;
    logger = nullptr;
    firstsearchLPA = true;
    firstsearchDstar = true;
}

Mission::Mission(const char* FileName) {
    fileName = FileName;
    logger = nullptr;
    firstsearchLPA = true;
    firstsearchDstar = true;
}

Mission::~Mission() {
    if (logger) {
        delete logger;
    }
}

bool Mission::getMap() {
    return map.getMap(fileName);
}

bool Mission::getConfig() {
    return config.getConfig(fileName);
}

bool Mission::createLog() {
    if (logger != nullptr) {
        delete logger;
    }
    logger = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    return logger->getLog(fileName, config.LogParams);
}

void Mission::createEnvironmentOptions() {
    options.cutcorners = config.SearchParams[CN_SP_CC];
    options.allowsqueeze = config.SearchParams[CN_SP_AS];
    options.allowdiagonal = config.SearchParams[CN_SP_AD];
    options.metrictype = config.SearchParams[CN_SP_MT];
    options.hweight = config.SearchParams[CN_SP_HW];
    options.breakingties = config.SearchParams[CN_SP_BT];
    options.searchtype = config.SearchParams[CN_SP_ST];
}

void Mission::createSearch() {
}

void Mission::printSearchResult(const SearchResult& sr) {
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

void Mission::startSearchLPAstar() {
    sr_Astar = search_Astar.startSearch(map, options);
    if (firstsearchLPA) {
        sr_LPAstar = search_LPAstar.startSearch(map, options);
        firstsearchLPA = false;
    } else {
        sr_LPAstar = search_LPAstar.repeat(map, options, changed);
    }
    std::cout << "\nA*:\n";
    printSearchResult(sr_Astar);
    std::cout << "LPA*:\n";
    printSearchResult(sr_LPAstar);
}

void Mission::startSearchAstar() {
    sr_Astar = search_Astar.startSearch(map, options);
    std::cout << "\nA*:\n";
    printSearchResult(sr_Astar);
    saveLog(sr_Astar, "_AStar", 2);
}

void Mission::runLPAstar(int cnt) {
    int i = 0;
    while (true) {
        startSearchLPAstar();
        saveLog(sr_LPAstar, "_TMP", 0);
        saveLog(sr_LPAstar, "_LPAStar", 1);
        saveLog(sr_Astar, "_AStar", 2);
        std::cout << "Results are saved via created log channel." << std::endl;
    waitingForChanges:
        if (i++ == cnt || !sr_Astar.pathfound) {
            return;
        }
        changeMap();
        if (changed.i == -1) {
            std::cout << "No changes found!" << std::endl;
            goto waitingForChanges;
        }
    }
}

void Mission::startSearchDstarLite() {
    sr_Astar = search_Astar.startSearch(map, options);
    std::cout << "\nA*:\n";
    printSearchResult(sr_Astar);
    if (firstsearchDstar) {
        sr_DstarLite = search_DstarLite.startSearch(map, options);
        firstsearchDstar = false;
    } else {
        sr_DstarLite = search_DstarLite.repeat(map, options, changed);
    }
    std::cout << "D* Lite:\n";
    printSearchResult(sr_DstarLite);
}

Cell Mission::scanMap(const Map& map, int i, int j) {
    int cnt = 0;
    Cell found(-1, -1);
    for (auto [x, y] : {
             Cell(-2, -2), Cell(-2, -1), Cell(-2, 0), Cell(-2, 1), Cell(-2, 2),
             Cell(-1, -2), Cell(-1, -1), Cell(-1, 0), Cell(-1, 1), Cell(-1, 2),
             Cell(0, -2),  Cell(0, -1),  Cell(0, 0),  Cell(0, 1),  Cell(0, 2),
             Cell(1, -2),  Cell(1, -1),  Cell(1, 0),  Cell(1, 1),  Cell(1, 2),
             Cell(2, -2),  Cell(2, -1),  Cell(2, 0),  Cell(2, 1),  Cell(2, 2),
         }) {
        if (map.getValue(i + x, j + y) != -1) {
            if (map.Grid[i + x][j + y] == 9) {
                found.i = i + x;
                found.j = j + y;
            }
        }
    }
    return found;
}

void Mission::mainDstarLite() {
    sr_Astar = search_Astar.startSearch(map, options);
    std::cout << "\nA*:\n";
    printSearchResult(sr_Astar);
    sr_DstarLite = search_DstarLite.startSearch(map, options);
    std::cout << "D* Lite:\n";
    printSearchResult(sr_DstarLite);
    std::cout << "\n";
    saveLog(sr_DstarLite, "_TMP", 0);
    saveLog(sr_DstarLite, "_DStarLite", 3);
    saveLog(sr_Astar, "_AStar", 2);

    changeMap();
    map.Grid[changed.i][changed.j] = 9;
    std::cout << "Changed: " << changed.i << " " << changed.j << "\n";

    auto curr = search_DstarLite.lppath.rbegin();
    while (search_DstarLite.start != search_DstarLite.goal) {
        search_DstarLite.start = &search_DstarLite.nodesMap[curr->i][curr->j];
        saveLog(sr_DstarLite, "_TMP", 4);
        saveLog(sr_DstarLite, "_DStarLite", 3);
        auto i = search_DstarLite.start->i;
        auto j = search_DstarLite.start->j;
        std::cout << "Moved to:" << i << " " << j << "\n";
        getchar();
        ++curr;

        Cell found = scanMap(map, i, j);
        if (found.i != -1 && found.j != -1) {
            std::cout << "Found: " << found.i << " " << found.j << "\n";
            map.Grid[found.i][found.j] = 1;
            Node* tmp = &search_DstarLite.nodesMap[found.i][found.j];
            search_DstarLite.nodesMap[tmp->i][tmp->j].g = std::numeric_limits<double>::infinity();
            search_DstarLite.nodesMap[tmp->i][tmp->j].rhs = std::numeric_limits<double>::infinity();
            search_DstarLite.nodesMap[tmp->i][tmp->j].parent = nullptr;

            map.start_i = i;
            map.start_j = j;

            sr_Astar = search_Astar.startSearch(map, options);
            std::cout << "\nA*:\n";
            printSearchResult(sr_Astar);
            sr_DstarLite = search_DstarLite.replan(map, options, tmp);
            std::cout << "D* Lite:\n";
            printSearchResult(sr_DstarLite);
            saveLog(sr_DstarLite, "_TMP", 0);
            saveLog(sr_DstarLite, "_DStarLite", 3);
            saveLog(sr_Astar, "_AStar", 2);

            if (!sr_DstarLite.pathfound) {
                return;
            }
            curr = search_DstarLite.lppath.rbegin();
            ++curr;
        }
    }
}

void Mission::runDstarLite(int cnt) {
    int i = 0;
    while (true) {
        startSearchDstarLite();
        saveLog(sr_DstarLite, "_TMP", 0);
        saveLog(sr_DstarLite, "_DStarLite", 3);
        saveLog(sr_Astar, "_AStar", 2);
        std::cout << "Results are saved via created log channel." << std::endl;
    waitingForChanges:
        if (i++ == cnt || !sr_Astar.pathfound) {
            return;
        }
        changeMap();
        if (changed.i == -1) {
            std::cout << "No changes found!" << std::endl;
            goto waitingForChanges;
        }
    }
}

int Mission::checkResult(const SearchResult& sr, float realLen) {
    if (!is_equal(options.hweight, 1)) {
        if (sr.pathlength <= options.hweight * realLen) {
            return 0;
        }
    } else {
        if (fabs(realLen - sr.pathlength) <= EPS) {
            return 0;
        }
    }
    std::cerr << "Incorrect path legth" << std::endl;
    std::cerr << "Your   : " << sr.pathlength << std::endl;
    std::cerr << "Correct: " << realLen << std::endl;
    return -1;
}

void Mission::changeMapAutomatically() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> tmp(2, search_LPAstar.nodesCntInPath - 1);
    int target = tmp(g), cnt = 1;
    for (const Node& node : *sr_LPAstar.lppath) {
        if (target == cnt) {
            changed.i = node.i;
            changed.j = node.j;
            break;
        }
        ++cnt;
    }
    map.Grid[changed.i][changed.j] = 1;
}

void Mission::changeMapAutomaticallyDstarLite() {
    std::random_device rd;
    std::mt19937 g(rd());
    if (search_DstarLite.nodesCntInPath < 7) {
        return;
    }
    int from = search_DstarLite.nodesCntInPath;
    from = from - (from / 10) * 2;
    int to = search_DstarLite.nodesCntInPath - 2;
    if (to - from < 3) {
        return;
    }
    std::uniform_int_distribution<> tmp(from, to);
    int target = tmp(g), cnt = 1;
    for (const Node& node : *sr_DstarLite.lppath) {
        if (target == cnt) {
            changed.i = node.i;
            changed.j = node.j;
            break;
        }
        ++cnt;
    }
    map.Grid[changed.i][changed.j] = 9;
}

bool Mission::testLPAstar(int cnt) {
    for (int i = 0; i < cnt; ++i) {
        changeMapAutomatically();
        startSearchLPAstar();
        if (sr_LPAstar.pathfound != sr_Astar.pathfound) {
            return 1;
        }
        if (!sr_Astar.pathfound) {
            return 0;
        }
        if (checkResult(sr_LPAstar, sr_Astar.pathlength) == -1) {
            return 1;
        }
    }
    return 0;
}

bool Mission::testDstarLite(int cnt) {
    changeMapAutomaticallyDstarLite();
    bool inFound = true;
    --cnt;
    auto curr = search_DstarLite.lppath.rbegin();
    while (search_DstarLite.start != search_DstarLite.goal) {
        search_DstarLite.start = &search_DstarLite.nodesMap[curr->i][curr->j];
        auto i = search_DstarLite.start->i;
        auto j = search_DstarLite.start->j;
        ++curr;

        Cell found = scanMap(map, i, j);
        if (found.i != -1 && found.j != -1) {
            map.Grid[found.i][found.j] = 1;
            Node* tmp = &search_DstarLite.nodesMap[found.i][found.j];
            search_DstarLite.nodesMap[tmp->i][tmp->j].g = std::numeric_limits<double>::infinity();
            search_DstarLite.nodesMap[tmp->i][tmp->j].rhs = std::numeric_limits<double>::infinity();
            search_DstarLite.nodesMap[tmp->i][tmp->j].parent = nullptr;

            map.start_i = i;
            map.start_j = j;

            sr_Astar = search_Astar.startSearch(map, options);
            std::cout << "\nA*:\n";
            printSearchResult(sr_Astar);

            sr_DstarLite = search_DstarLite.replan(map, options, tmp);
            std::cout << "D* Lite:\n";
            printSearchResult(sr_DstarLite);

            if (sr_DstarLite.pathfound != sr_Astar.pathfound) {
                return 1;
            }
            if (!sr_Astar.pathfound) {
                return 0;
            }
            if (checkResult(sr_DstarLite, sr_Astar.pathlength) == -1) {
                return 1;
            }

            inFound = false;
            if (cnt > 0 && !inFound) {
                changeMapAutomaticallyDstarLite();
                inFound = true;
                --cnt;
            }
            curr = search_DstarLite.lppath.rbegin();
            ++curr;
        }
    }
    return 0;
}

void Mission::changeMap() {
    std::cout << "Waiting for changes\n\n";

    struct stat result;
    if (stat(fileName, &result) == -1) {
        std::cout << "stat error!" << std::endl;
        exit(1);
    }
    time_t lastModTime = result.st_mtime;
    while (true) {
#ifndef WIN32
        sleep(1);
#endif
#ifdef WIN32
        Sleep(1000);
#endif
        if (stat(fileName, &result) == -1) {
            std::cout << "stat error!" << std::endl;
            exit(1);
        }
        if (lastModTime != result.st_mtime) {
            break;
        }
    }

    map.prevGrid.resize(map.getMapHeight(), std::vector<int>(map.getMapWidth(), 0));
    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            map.prevGrid[i][j] = map.getValue(i, j);
        }
    }
    map.deleteGrid();
    map.getMap(fileName);

    changed.i = -1;
    changed.j = -1;
    for (int i = 0; i < map.getMapHeight(); ++i) {
        for (int j = 0; j < map.getMapWidth(); ++j) {
            if (map.getValue(i, j) != map.prevGrid[i][j]) {
                changed.i = i;
                changed.j = j;
                return;
            }
        }
    }
}

void Mission::saveLog(const SearchResult& sr, const char* nameSuffix, int searchType) {
    if (logger != nullptr) {
        delete logger;
    }
    logger = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    logger->getLog(fileName, config.LogParams);
    logger->writeToLogSummary(sr.numberofsteps,
                              sr.nodescreated,
                              sr.pathlength,
                              sr.time,
                              map.getCellSize());
    if (sr.pathfound) {
        logger->writeToLogPath(*sr.lppath);
        logger->writeToLogHPpath(*sr.hppath);
        if (searchType == 0) {
            logger->writeToLogMap(map, *sr.lppath);
        } else if (searchType == 1) {
            logger->writeToLogOpen(map, search_LPAstar);
        } else if (searchType == 2) {
            logger->writeToLogOpen(map, search_Astar);
        } else if (searchType == 3) {
            logger->writeToLogOpen(map, search_DstarLite);
        } else if (searchType == 4) {
            logger->writeToLogMap(map, *sr.lppath, search_DstarLite.start, search_DstarLite.goal);
        }
    } else {
        logger->writeToLogNotFound();
    }
    std::string str = logger->getLogFilename();
    std::string last;
    for (size_t i = str.size() - 4; i < str.size(); ++i) {
        last.push_back(str[i]);
    }
    str.resize(str.size() - 4);
    str.append(nameSuffix);
    str.append(last);
    logger->saveLog(str);
}

SearchResult Mission::getLPAstarSearchResult() {
    return sr_LPAstar;
}

SearchResult Mission::getAstarSearchResult() {
    return sr_Astar;
}

SearchResult Mission::getDstarLiteSearchResult() {
    return sr_DstarLite;
}
