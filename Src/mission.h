#ifndef MISSION_H
#define MISSION_H

#include "config.h"
#include "environmentoptions.h"
#include "ilogger.h"
#include "map.h"
#include "Astar.h"
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
    void createSearch();
    void createEnvironmentOptions();
    void startSearch();
    void printSearchResultsToConsole();
    void saveSearchResultsToLog();
    SearchResult getSearchResult();

   private:
    Map map;
    Config config;
    EnvironmentOptions options;
    Astar search_Astar;
    ILogger* logger;
    const char* fileName;
    SearchResult sr_Astar;
};

#endif
