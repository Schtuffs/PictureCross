#pragma once

#include "Grid.h"
#include "HeaderInfo.h"

class Solver {
private:
    HeaderInfo mInfo;

public:
    Solver(const HeaderInfo& info);

    Grid solve();
    
    ~Solver();
};

