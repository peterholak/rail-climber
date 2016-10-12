#pragma once

#include <string>
#include <list>

struct SvgPath {
    std::string type;
    std::string transform;
    std::string d;
    std::string style;
    double cx = 0.0, cy = 0.0;

    std::list<std::pair<double, double>> parsed;
};
