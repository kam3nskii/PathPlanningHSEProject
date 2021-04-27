#ifndef XMLLOGGER_H
#define XMLLOGGER_H
#include "ilogger.h"
#include "tinyxml2.h"

class XmlLogger : public ILogger {
   public:
    XmlLogger(std::string loglevel) : ILogger(loglevel) {
    }
    virtual ~XmlLogger(){};

    bool getLog(const char* FileName, const std::string* LogParams);
    void saveLog();
    void saveLog(std::string logName);
    std::string getLogFilename();
    void writeToLogMap(const Map& map, const std::list<Node>& path);
    void writeToLogOpen(const Map& map, const Astar& search);
    void writeToLogOpen(const Map& map, const LPAstar& search);
    void writeToLogPath(const std::list<Node>& path);
    void writeToLogHPpath(const std::list<Node>& hppath);
    void writeToLogNotFound();
    void writeToLogSummary(unsigned int numberofsteps, unsigned int nodescreated, float length,
                           double time, double cellSize);

    std::string LogFileName;
    tinyxml2::XMLDocument doc;
};

#endif
