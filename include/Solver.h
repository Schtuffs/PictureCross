#pragma once

#include "Grid.h"
#include "HeaderInfo.h"
#include "Line.h"
#include "Defines.h"

class Solver {
public:
    Solver(const HeaderInfo& info);

    const Grid& solve();
    // Returns the runtime for the previous solve
    double runtime() const noexcept;
    
    ~Solver();
    
private:
    HeaderInfo mInfo;
    Grid mGrid;
    unsigned char mSolvedLines, mColCount, mRowCount, mColSize, mRowSize;
    double mRuntime;

    // ----- Read -----

    bool isComplete();
    bool isComplete(const Line& line) const noexcept;

    // ----- Gathering -----

    // Find how many sections are in the line
    inline int lineSections(const Line& line) const noexcept;

    // Splits line into the smaller sections based on expected sections
    Array<Line> lineSplit(const Line& line) const noexcept;

    // ----- Update -----
    
    // Fills in specified area
    void fill(TYPE t, int line, int start, int spaces, STATE state) noexcept;

    // ----- Initializing -----

    // Fills in the beginning header data
    void initGrid() noexcept;
    // Fills in the data when initializing grid with complete line
    inline void initCompleteLine(TYPE type, int lineNum, const Line& line) noexcept;
    // Fills in as much data as possible when initializing grid with incomplete line
    inline void initIncompleteLine(TYPE type, int lineNum, int remain, const Line& line) noexcept;
    
    // ----- Looping -----
    
    // Checks a line and performs necessary logic to complete it more
    void check(TYPE type, int lineNum, const Line& line) noexcept;
    
    // Solves an individual line section
    void lineSectionSolve(TYPE type, int lineNum, const Line& line) noexcept;

    // Completes line that has all needed known values
    inline void completeLineSection(TYPE type, int lineNum, const Line& line) noexcept;
    // Fills in as much data as possible for incomplete line
    inline void incompleteLineSection(TYPE type, int lineNum, int remain, const Line& line) noexcept;
};

