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
#include <cmath>
#include <random>

#include "Astar.h"
#include "LPAstar.h"
#include "DstarLite.h"
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
    void startSearchAstar();
    void startSearchLPAstar();
    void startSearchDstarLite();
    void changeMap();
    Cell scanMap(const Map& map, int i, int j);
    void changeMapAutomatically();
    void mainDstarLite();
    void changeMapAutomaticallyDstarLite();
    void runLPAstar(int cnt);
    bool testLPAstar(int cnt);
    bool testDstarLite(int cnt);
    void runDstarLite(int cnt);
    int checkResult(const SearchResult& sr, float realLen);
    void saveLog(const SearchResult& sr, const char* nameSuffix, int searchType);
    void printSearchResult(const SearchResult& sr);
    SearchResult getLPAstarSearchResult();
    SearchResult getAstarSearchResult();
    SearchResult getDstarLiteSearchResult();

    Map map;
    Config config;
    EnvironmentOptions options;
    Astar search_Astar;
    LPAstar search_LPAstar;
    DstarLite search_DstarLite;
    ILogger* logger;
    const char* fileName;
    SearchResult sr_Astar;
    SearchResult sr_LPAstar;
    SearchResult sr_DstarLite;
    Cell changed;
    bool firstsearchLPA;
    bool firstsearchDstar;
};

#endif
