#pragma once

#include <iostream>

#include "Array.h"

enum STATE { NONE, VALID, INVALID, UNKNOWN };

class Grid {
private:
    Array<Array<STATE>> mCells;

public:
    Grid(int cols, int rows);
    ~Grid();

    // Sets specified cell to given state
    bool set(int col, int row, STATE state) noexcept;

    // Gets specified column from grid
    // Returns first column if out of bounds
    const Array<STATE> col(int index) const noexcept;

    // Gets specified row from grid
    // Returns first row if out of bounds
    const Array<STATE>& row(int index) const noexcept;

    // Gets the state of the specified cell
    STATE get(int col, int row) const noexcept;

    friend std::ostream& operator<<(std::ostream& cout, const Grid& grid);
};

