#ifndef ENVIRONMENTOPTIONS_H
#define ENVIRONMENTOPTIONS_H
#include "gl_const.h"

class EnvironmentOptions {
   public:
    double hweight;
    bool allowdiagonal;
    bool cutcorners;
    bool allowsqueeze;
    int searchtype;
    int metrictype;
    int breakingties;
};

#endif
