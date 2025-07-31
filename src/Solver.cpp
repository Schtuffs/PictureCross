#include "Solver.h"

Solver::Solver(const HeaderInfo& info) : mInfo(info) {}
Solver::~Solver() {}

Grid Solver::solve() {
    Grid grid(this->mInfo.col(), this->mInfo.row());
    return grid;
}

