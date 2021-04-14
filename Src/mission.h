#ifndef MISSION_H
#define MISSION_H

#include <algorithm>
#include <cmath>

#include "config.h"
#include "environmentoptions.h"
#include "ilogger.h"
#include "map.h"
#include "Astar.h"
#include "LPAstar.h"
#include "searchresult.h"
#include "xmllogger.h"

class Mission {
   public:
    Mission();
    Mission(const char* fileName);
    ~Mission();

    bool getMap();
    bool getConfig();
    bool createLog();
    bool testLogCreate();
    void testLogSave();
    void createSearch();
    void createEnvironmentOptions();
    void startSearch();
    void startSecondSearch();
    void saveSearchResultsToLogLPAStar();
    void saveSearchResultsToLogLPAStarSecond();
    void saveSearchResultsToLogSecondAstarSecond();
    SearchResult getSearchResult();

//    private:
    Map map;
    Map mapSecond;
    Config config;
    EnvironmentOptions options;
    Astar search_Astar;
    Astar search_AstarSecond;
    LPAstar search_LPAstar;
    ILogger* logger;
    ILogger* logger2;
    ILogger* logger3;
    ILogger* someLogger;
    const char* fileName;
    SearchResult sr_Astar;
    SearchResult sr_AstarSecond;
    SearchResult sr_LPAstar;
    SearchResult sr_LPAstarSecond;
};

#endif
