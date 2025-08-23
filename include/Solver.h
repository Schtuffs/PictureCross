#pragma once

#include "Grid.h"
#include "HeaderInfo.h"
#include "Line.h"
#include "Defines.h"

typedef struct {
    TYPE type;
    int lineNum;
    const Line& line;
} LineData;

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
    Array<bool> mSolvedCols, mSolvedRows;
    double mRuntime;

    void reset();

    // ----- Read -----

    bool isComplete();
    bool isComplete(const LineData& line) noexcept;

    // ----- Gathering -----

    // Find how many sections are in the line
    int lineSections(const Line& line) const noexcept;

    // Splits line into the smaller sections based on expected sections
    Array<Line> lineSplit(const Line& line) const noexcept;

    // Marks given line as completed
    void markCompletion(const LineData& data) noexcept;

    // ----- Update -----
    
    // Fills in specified area
    void fill(TYPE t, int line, int start, int spaces, STATE state) noexcept;

    // ----- Initializing -----

    // Fills in the beginning header data
    void initGrid() noexcept;
    // Fills in the data when initializing grid with complete line
    void initCompleteLine(const LineData& data) noexcept;
    // Fills in as much data as possible when initializing grid with incomplete line
    void initIncompleteLine(const LineData& data, int remain) noexcept;
    
    // ----- Looping -----
    
    // Checks a line and performs necessary logic to complete it more
    void check(const LineData& data) noexcept;

    // Removes any areas in the line that are too small to hold the required data
    void removeSmallSections(const LineData& data) noexcept;
    
    // Solves an individual line section
    void lineSectionSolve(const LineData& data) noexcept;

    // Checks edges of line for possible data to fill
    void lineEdgeCheck(const LineData& data) noexcept;

    // Completes line that has all needed known values
    void completeLineSection(const LineData& data) noexcept;
    // Fills in as much data as possible for incomplete line
    void incompleteLineSection(const LineData& data, int remain) noexcept;
};

