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
    double runtime() const;
    
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
    bool isComplete(const LineData& line);

    // ----- Gathering -----

    // Find how many sections are in the line
    int lineSections(const Line& line) const;

    // Splits line into the smaller sections based on expected sections
    Array<Line> lineSplit(const Line& line) const;

    // Marks given line as completed
    void markCompletion(const LineData& data);

    // ----- Update -----
    
    // Fills in specified area
    void fill(TYPE t, int line, int start, int spaces, STATE state);

    // ----- Initializing -----

    // Fills in the beginning header data
    void initGrid();
    // Fills in the data when initializing grid with complete line
    void initCompleteLine(const LineData& data);
    // Fills in as much data as possible when initializing grid with incomplete line
    void initIncompleteLine(const LineData& data, int remain);
    
    // ----- Looping -----
    
    // Checks a line and performs necessary logic to complete it more
    void check(const LineData& data);

    // Removes any areas in the line that are too small to hold the required data
    void removeSmallSections(const LineData& data);
    
    // Solves an individual line section
    void lineSectionSolve(const LineData& data);

    // Checks edges of line for possible data to fill
    void lineEdgeCheck(const LineData& data);

    // Fills in edge data
    void fillEdge(const LineData& data, int startIndex, int endIndex, int validIndex, bool isLeft);

    // Completes line that has all needed known values
    void completeLineSection(const LineData& data);
    // Fills in as much data as possible for incomplete line
    void incompleteLineSection(const LineData& data, int remain);
};

