#pragma once

#include <iostream>

#include "Array.h"

enum STATE { NONE, VALID, INVALID };

class Grid {
private:
    Array<Array<STATE>> mCells;

public:
    Grid(int cols, int rows);
    ~Grid();

    bool set(int col, int row, STATE state);

    // const Array<STATE>& cols(int index) const noexcept;
    // Gets the specified row from grid
    // Returns first row if out of bounds
    const Array<STATE>& row(int index) const noexcept;

    friend std::ostream& operator<<(std::ostream& cout, const Grid& grid);
};

