#pragma once

#include "Grid.h"
#include "HeaderInfo.h"

class Solver {
private:
    enum TYPE { COL, ROW };
    HeaderInfo mInfo;
    Grid mGrid;
    int mSolvedLines;
    double mRuntime;

    // ----- Read -----

    bool isComplete();

    // ----- Update -----

    // Fills in specified area
    void fill(TYPE t, int line, int start, int spaces, STATE state);

    // Fills in the beginning header data
    void initGrid();
    // Fills in the data when initializing grid with complete line
    inline void initCompleteLine(TYPE type, int lineNum, const Array<int>& line) noexcept;
    // Fills in as much data as possible when initializing grid with incomplete line
    inline void initIncompleteLine(TYPE type, int lineNum, int remain, const Array<int>& line) noexcept;

public:
    Solver(const HeaderInfo& info);

    const Grid& solve();
    // Returns the runtime for the previous solve
    double runtime() const noexcept;
    
    ~Solver();
};

