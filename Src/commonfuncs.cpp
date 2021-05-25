#include "commonfuncs.h"

bool is_equal(double x, double y) {
    if (x == std::numeric_limits<double>::infinity() &&
        y != std::numeric_limits<double>::infinity()) {
        return false;
    }
    if (y == std::numeric_limits<double>::infinity() &&
        x != std::numeric_limits<double>::infinity()) {
        return false;
    }
    if (x == std::numeric_limits<double>::infinity() &&
        y == std::numeric_limits<double>::infinity()) {
        return true;
    }
    
    return std::fabs(x - y) < 0.000001;
}

bool isLess(const std::pair<double, double>& x, const std::pair<double, double>& y) {
    if (x.first == std::numeric_limits<double>::infinity()) {
        if (y.first == std::numeric_limits<double>::infinity()) {
            goto check_second;
        } else {
            return false;
        }
    } else {
        if (y.first == std::numeric_limits<double>::infinity()) {
            return true;
        } else {
            if (is_equal(x.first, y.first)) {
                goto check_second;
            } else if (x.first < y.first) {
                return true;
            } else if (x.first > y.first) {
                return false;
            }
        }
    }

check_second:
    if (x.second == std::numeric_limits<double>::infinity()) {
        if (y.second == std::numeric_limits<double>::infinity()) {
            return false;
        } else {
            return false;
        }
    } else {
        if (y.second == std::numeric_limits<double>::infinity()) {
            return true;
        } else {
            if (is_equal(x.second, y.second)) {
                return false;
            } else if (x.second < y.second) {
                return true;
            } else if (x.second > y.second) {
                return false;
            }
        }
    }
}
