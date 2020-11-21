#ifndef MAP_H
#define MAP_H
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "environmentoptions.h"
#include "gl_const.h"
#include "node.h"
#include "tinyxml2.h"

//That's the class that stores BOTH grid map data AND start-goal locations.
//getValue reads the input XML and fills the Map object.
//Some naive checks are already included in getValue but still on some corrupted XMLs it may fail,
//so be sure to invoke it passing the correct XML (e.g. with correctly filled map tag, e.g. <map>.

//Map is NOT to be modified during the search. It should be passed as a const pointer.
//Think of it as an "independent" piece of data that is managed by outer (non-search related) proccesses.
//Search algorithm should create it own object/structures needed to run the search on that map.

class Map {
private:
    int height, width;
    int start_i, start_j;
    int goal_i, goal_j;
    double cellSize;
    int** Grid;

public:
    Map();
    Map(const Map& orig);
    ~Map();

    bool getMap(const char* FileName);
    bool CellIsTraversable(int i, int j) const;
    bool CellOnGrid(int i, int j) const;
    bool CellIsObstacle(int i, int j) const;
    int getValue(int i, int j) const;
    int getMapHeight() const;
    int getMapWidth() const;
    int getStart_i() const;
    int getStart_j() const;
    int getGoal_i() const;
    int getGoal_j() const;
    double getCellSize() const;
    double getTransitionCost(int i1, int j1, int i2, int j2) const;
    std::vector<Node> getNeighbors(Node node, const EnvironmentOptions& options) const;
};

#endif
