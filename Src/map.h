#ifndef MAP_H
#define MAP_H
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "cell.h"
#include "environmentoptions.h"
#include "gl_const.h"
#include "node.h"
#include "tinyxml2.h"

class Map {
   private:
    int height, width;
    int start_i, start_j;
    int goal_i, goal_j;
    double cellSize;

   public:
    int** Grid;
    std::vector<std::vector<int>> prevGrid;
    Map();
    Map(const Map& orig);
    ~Map();

    void deleteGrid();
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
    std::vector<Cell> getNeighbors(const Node& node, const EnvironmentOptions& options) const;
};

#endif
