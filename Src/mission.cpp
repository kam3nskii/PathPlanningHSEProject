#include "mission.h"
#include "cell.h"

#include <iomanip>
#include <iostream>

Mission::Mission() {
    logger = nullptr;
    logger2 = nullptr;
    logger3 = nullptr;
    fileName = nullptr;
    someLogger = nullptr;
}

Mission::Mission(const char* FileName) {
    fileName = FileName;
    logger = nullptr;
    logger2 = nullptr;
    logger3 = nullptr;
    someLogger = nullptr;
}

Mission::~Mission() {
    if (logger)
        delete logger;
    if (logger2)
        delete logger2;
    if (logger3)
        delete logger3;
    if (someLogger)
        delete someLogger;
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
    logger->getLog(fileName, config.LogParams);
    if (logger2 != nullptr) {
        delete logger2;
    }
    logger2 = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    logger2->getLog(fileName, config.LogParams);
    if (logger3 != nullptr) {
        delete logger3;
    }
    logger3 = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    return logger3->getLog(fileName, config.LogParams);
}

bool Mission::testLogCreate() {
    someLogger = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    return someLogger->getLog(fileName, config.LogParams);
}

void Mission::testLogSave() {
    // someLogger->writeToLogSummary(sr_Astar.numberofsteps,
    //                           sr_Astar.nodescreated,
    //                           sr_Astar.pathlength,
    //                           sr_Astar.time,
    //                           map.getCellSize());
    // if (sr_Astar.pathfound) {
    //     // someLogger->writeToLogPath(*sr_Astar.lppath);
    //     // someLogger->writeToLogHPpath(*sr_Astar.hppath);
    // } else {
    //     someLogger->writeToLogNotFound();
    // }
    someLogger->writeToLogOpen(map, search_LPAstar);
    someLogger->saveLog();
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
    // might be helpful in case numerous algorithms are added
}

void Mission::startSearch() {
    sr_Astar = search_Astar.startSearch(map, options);
    sr_LPAstar = search_LPAstar.startSearch(map, options);

    std::cout << "\nA*:\n";
    std::cout << "Path ";
    if (!sr_Astar.pathfound) {
        std::cout << "NOT ";
    }
    std::cout << "found!" << std::endl;
    std::cout << "numberofsteps=" << sr_Astar.numberofsteps << std::endl;
    std::cout << "nodescreated=" << sr_Astar.nodescreated << std::endl;
    if (sr_Astar.pathfound) {
        std::cout.setf(std::ios::fixed);
        std::cout.precision(5);
        std::cout << "pathlength=" << sr_Astar.pathlength << std::endl;
        std::cout << "pathlength_scaled=" << sr_Astar.pathlength * map.getCellSize() << std::endl;
    }
    std::cout.precision(10);
    std::cout << "time=" << sr_Astar.time << std::endl;

    //
    std::cout << "\nLPA*:\n";
    std::cout << "Path ";
    if (!sr_LPAstar.pathfound) {
        std::cout << "NOT ";
    }
    std::cout << "found!" << std::endl;
    std::cout << "numberofsteps=" << sr_LPAstar.numberofsteps << std::endl;
    std::cout << "nodescreated=" << sr_LPAstar.nodescreated << std::endl;
    if (sr_LPAstar.pathfound) {
        std::cout.setf(std::ios::fixed);
        std::cout.precision(5);
        std::cout << "pathlength=" << sr_LPAstar.pathlength << std::endl;
        std::cout << "pathlength_scaled=" << sr_LPAstar.pathlength * map.getCellSize() << std::endl;
    }
    std::cout.precision(10);
    std::cout << "time=" << sr_LPAstar.time << std::endl;
}

void Mission::startSecondSearch() {
    std::cout << "waiting x and y: ";
    int x, y;
    std::cin >> y >> x;

    mapSecond.getMap(fileName);

    Cell changed(x, y);

    sr_LPAstarSecond = search_LPAstar.repeat(mapSecond, options, changed);
    sr_AstarSecond = search_AstarSecond.startSearch(mapSecond, options);

    std::cout << "\nLPA*:\n";
    std::cout << "Path ";
    if (!sr_LPAstarSecond.pathfound) {
        std::cout << "NOT ";
    }
    std::cout << "found!" << std::endl;
    std::cout << "numberofsteps=" << sr_LPAstarSecond.numberofsteps << std::endl;
    std::cout << "nodescreated=" << sr_LPAstarSecond.nodescreated << std::endl;
    if (sr_LPAstarSecond.pathfound) {
        std::cout.setf(std::ios::fixed);
        std::cout.precision(5);
        std::cout << "pathlength=" << sr_LPAstarSecond.pathlength << std::endl;
        std::cout << "pathlength_scaled=" << sr_LPAstarSecond.pathlength * map.getCellSize() << std::endl;
    }
    std::cout.precision(10);
    std::cout << "time=" << sr_LPAstarSecond.time << std::endl;

    std::cout << "\nA*:\n";
    std::cout << "Path ";
    if (!sr_AstarSecond.pathfound) {
        std::cout << "NOT ";
    }
    std::cout << "found!" << std::endl;
    std::cout << "numberofsteps=" << sr_AstarSecond.numberofsteps << std::endl;
    std::cout << "nodescreated=" << sr_AstarSecond.nodescreated << std::endl;
    if (sr_AstarSecond.pathfound) {
        std::cout.setf(std::ios::fixed);
        std::cout.precision(5);
        std::cout << "pathlength=" << sr_AstarSecond.pathlength << std::endl;
        std::cout << "pathlength_scaled=" << sr_AstarSecond.pathlength * map.getCellSize() << std::endl;
    }
    std::cout.precision(10);
    std::cout << "time=" << sr_AstarSecond.time << std::endl;
}

void Mission::saveSearchResultsToLogLPAStar() {
    logger->writeToLogSummary(sr_LPAstar.numberofsteps,
                              sr_LPAstar.nodescreated,
                              sr_LPAstar.pathlength,
                              sr_LPAstar.time,
                              map.getCellSize());
    if (sr_LPAstar.pathfound) {
        logger->writeToLogPath(*sr_LPAstar.lppath);
        logger->writeToLogHPpath(*sr_LPAstar.hppath);
        logger->writeToLogMap(map, *sr_LPAstar.lppath);
    } else {
        logger->writeToLogNotFound();
    }
    std::string str = logger->getLogFilename();
    std::string last;
    for (size_t i = str.size() - 4; i < str.size(); ++i) {
        last.push_back(str[i]);
    }
    str.resize(str.size() - 4);
    str.append("_LPAStar");
    str.append(last);
    logger->saveLog(str);
}

void Mission::saveSearchResultsToLogLPAStarSecond() {
    logger2->writeToLogSummary(sr_LPAstarSecond.numberofsteps,
                              sr_LPAstarSecond.nodescreated,
                              sr_LPAstarSecond.pathlength,
                              sr_LPAstarSecond.time,
                              mapSecond.getCellSize());
    if (sr_LPAstarSecond.pathfound) {
        logger2->writeToLogPath(*sr_LPAstarSecond.lppath);
        logger2->writeToLogHPpath(*sr_LPAstarSecond.hppath);
        // logger2->writeToLogMap(mapSecond, *sr_LPAstarSecond.lppath);
        logger2->writeToLogOpen(mapSecond, search_LPAstar);
    } else {
        logger2->writeToLogNotFound();
    }
    std::string str = logger2->getLogFilename();
    std::string last;
    for (size_t i = str.size() - 4; i < str.size(); ++i) {
        last.push_back(str[i]);
    }
    str.resize(str.size() - 4);
    str.append("_LPAStarSecond");
    str.append(last);
    logger2->saveLog(str);
}

void Mission::saveSearchResultsToLogSecondAstarSecond() {
    logger3->writeToLogSummary(sr_AstarSecond.numberofsteps,
                              sr_AstarSecond.nodescreated,
                              sr_AstarSecond.pathlength,
                              sr_AstarSecond.time,
                              mapSecond.getCellSize());
    if (sr_AstarSecond.pathfound) {
        logger3->writeToLogPath(*sr_AstarSecond.lppath);
        logger3->writeToLogHPpath(*sr_AstarSecond.hppath);
        // logger3->writeToLogMap(mapSecond, *sr_AstarSecond.lppath);
        logger3->writeToLogOpen(mapSecond, search_AstarSecond);
    } else {
        logger3->writeToLogNotFound();
    }
    std::string str = logger3->getLogFilename();
    std::string last;
    for (size_t i = str.size() - 4; i < str.size(); ++i) {
        last.push_back(str[i]);
    }
    str.resize(str.size() - 4);
    str.append("_AStarSecond");
    str.append(last);
    logger3->saveLog(str);
}

SearchResult Mission::getSearchResult() {
    //return sr_Astar;
    return sr_LPAstar;
}
