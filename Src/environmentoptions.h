#ifndef ENVIRONMENTOPTIONS_H
#define ENVIRONMENTOPTIONS_H
#include "gl_const.h"

class EnvironmentOptions {
public:
    int metrictype;      //Can be chosen Euclidean, Manhattan, Chebyshev and Diagonal distance
    bool allowsqueeze;   //Option that allows to move throught "bottleneck"
    bool allowdiagonal;  //Option that allows to make diagonal moves
    bool cutcorners;     //Option that allows to make diagonal moves, when one adjacent cell is untraversable
    double hweight;      //Can be chosen >= 1
    int breakingties;    //Can be chosen g-min, g-max
    int searchtype;      //Can be choose dijkstra, astar
};

#endif  // ENVIRONMENTOPTIONS_H
