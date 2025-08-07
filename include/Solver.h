#pragma once

#include "Grid.h"
#include "HeaderInfo.h"
#include "Line.h"
#include "Defines.h"

class Solver {
private:
    HeaderInfo mInfo;
    Grid mGrid;
    char mSolvedLines, mColCount, mRowCount, mColSize, mRowSize;
    double mRuntime;

    // ----- Read -----

    bool isComplete();
    bool isComplete(const Line& line) const noexcept;

    // ----- Gathering -----

    // Gets the total open areas for the section
    inline int lineSections(const Line& line) const noexcept;

    // ----- Update -----

    // Fills in specified area
    void fill(TYPE t, int line, int start, int spaces, STATE state);

    // ----- Initializing -----

    // Fills in the beginning header data
    void initGrid();
    // Fills in the data when initializing grid with complete line
    inline void initCompleteLine(TYPE type, int lineNum, const Line& line) noexcept;
    // Fills in as much data as possible when initializing grid with incomplete line
    inline void initIncompleteLine(TYPE type, int lineNum, int remain, const Line& line) noexcept;

    // ----- Looping -----

    // Checks a line and performs necessary logic to complete it more
    void check(TYPE type, const Line& line);
public:
    Solver(const HeaderInfo& info);

    const Grid& solve();
    // Returns the runtime for the previous solve
    double runtime() const noexcept;
    
    ~Solver();
};

