#ifndef ENVIRONMENTOPTIONS_H
#define ENVIRONMENTOPTIONS_H
#include "gl_const.h"

class EnvironmentOptions {
   public:
    int metrictype;
    bool allowsqueeze;
    bool allowdiagonal;
    bool cutcorners;
    double hweight;
    int breakingties;
    int searchtype;
};

#endif
