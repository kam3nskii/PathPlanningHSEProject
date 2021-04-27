#ifndef MISSION_H
#define MISSION_H

#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#include <windows.h>
#define stat _stat
#endif

#include <algorithm>
#include <random>
#include <cmath>

#include "Astar.h"
#include "LPAstar.h"
#include "config.h"
#include "environmentoptions.h"
#include "ilogger.h"
#include "map.h"
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
    void changeMap();
    void changeMapAutomatically();
    void run(int cnt);
    bool test(int cnt);

    int checkResult(const SearchResult& sr, float realLen);
    void saveLog(const SearchResult& sr, const char* nameSuffix, int searchType);

    void printSearchResult(const SearchResult& sr);
    SearchResult getLPAstarSearchResult();
    SearchResult getAstarSearchResult();

    //    private:
    Map map;
    Config config;
    EnvironmentOptions options;
    Astar search_Astar;
    LPAstar search_LPAstar;
    ILogger* logger;
    const char* fileName;
    SearchResult sr_Astar;
    SearchResult sr_LPAstar;
    Cell changed;
    bool firstsearch;
    int debugcnt = 0;
    // std::vector<int> debugarr = {10, 3, 8, 17, 3, 17, 15, 2, 15, 3, 12, 10, 17, 18, 30, 30};
    std::vector<int> debugarr = {6};
};

#endif
