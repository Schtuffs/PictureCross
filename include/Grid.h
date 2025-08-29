#pragma once

#include <iostream>

#include "Array.h"
#include "Line.h"

class Grid {
private:
    Array<Array<STATE>> mCells;
    Array<Line> mCols, mRows;

public:
    Grid(const Array<Array<int>>& colInfo, const Array<Array<int>>& rowInfo);
    ~Grid();

    // Gets specified column from grid
    // Returns first column if out of bounds
    const Line& col(int index) const;

    // Gets specified row from grid
    // Returns first row if out of bounds
    const Line& row(int index) const;

    // Gets the state of the specified cell
    STATE get(int col, int row) const;

    // Sets the header data for the given column or row
    bool set(int col, int row, STATE state);

    friend std::ostream& operator<<(std::ostream& cout, const Grid& grid);
};

