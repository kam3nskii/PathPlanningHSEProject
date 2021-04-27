#include "mission.h"

#include <iomanip>
#include <iostream>

#include "cell.h"

Mission::Mission() {
    fileName = nullptr;
    logger = nullptr;
    firstsearch = true;
}

Mission::Mission(const char* FileName) {
    fileName = FileName;
    logger = nullptr;
    firstsearch = true;
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

void Mission::startSearch() {
    sr_Astar = search_Astar.startSearch(map, options);
    if (firstsearch) {
        sr_LPAstar = search_LPAstar.startSearch(map, options);
        firstsearch = false;
    } else {
        sr_LPAstar = search_LPAstar.repeat(map, options, changed);
    }
    std::cout << "A*:\n";
    printSearchResult(sr_Astar);
    std::cout << "LPA*:\n";
    printSearchResult(sr_LPAstar);
    std::cout << "\n";
}

void Mission::run(int cnt) {
    int i = 0;
    while (true) {
        startSearch();
        saveLog(sr_LPAstar, "_LPAStarTMP", 0);
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

bool Mission::test(int cnt) {
    for (int i = 0; i < cnt; ++i) {
        // getchar();
        changeMapAutomatically();
        startSearch();
        // saveLog(sr_LPAstar, "_LPAStarTMP", 0);
        // saveLog(sr_LPAstar, "_LPAStar", 1);
        // saveLog(sr_Astar, "_AStar", 2);
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
