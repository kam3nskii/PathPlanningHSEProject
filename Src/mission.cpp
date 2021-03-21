#include "mission.h"

#include <iomanip>
#include <iostream>

Mission::Mission() {
    logger = nullptr;
    fileName = nullptr;
}

Mission::Mission(const char* FileName) {
    fileName = FileName;
    logger = nullptr;
}

Mission::~Mission() {
    if (logger)
        delete logger;
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
    // might be helpful in case numerous algorithms are added
}

void Mission::startSearch() {
    sr_Astar = search_Astar.startSearch(logger, map, options);
}

void Mission::printSearchResultsToConsole() {
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
    std::cout.precision(7);
    std::cout << "time=" << sr_Astar.time << std::endl;
}

void Mission::saveSearchResultsToLog() {
    logger->writeToLogSummary(sr_Astar.numberofsteps,
                              sr_Astar.nodescreated,
                              sr_Astar.pathlength,
                              sr_Astar.time,
                              map.getCellSize());
    if (sr_Astar.pathfound) {
        logger->writeToLogPath(*sr_Astar.lppath);
        logger->writeToLogHPpath(*sr_Astar.hppath);
        logger->writeToLogMap(map, *sr_Astar.lppath);
    } else {
        logger->writeToLogNotFound();
    }
    logger->saveLog();
}

SearchResult Mission::getSearchResult() {
    return sr_Astar;
}
