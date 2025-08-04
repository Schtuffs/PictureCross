#include "Grid.h"

#include "Defines.h"

// ----- Creation ----- Destruction -----

Grid::Grid(int cols, int rows) : mCells(rows), mCols(cols), mRows(rows) {
    for (int i = 0; i < rows; i++) {
        this->mCells[i] = Array<STATE>(cols);
    }
}

Grid::~Grid() {}



// ----- Read -----

const Line Grid::col(int index) const noexcept {
    if (0 > index || index > this->mCols.size()) {
        return Line();
    }

    return this->mCols[index];
}

const Line Grid::row(int index) const noexcept {
    if (0 > index || index > (int)this->mCells.size()) {
        return Line();
    }
    return this->mRows[index];
}

STATE Grid::get(int col, int row) const noexcept {
    if (0 > col || col > this->mCells[0].size()) {
        return UNKNOWN;
    }
    if (0 > row || row > this->mCells.size()) {
        return UNKNOWN;
    }
    return this->mCells[row][col];
}



// ----- Update -----

bool Grid::set(int col, int row, STATE state) noexcept {
    // Check indexing
    if (0 > col || col > this->mCells[0].size()) {
        return false;
    }
    if (0 > row || row > this->mCells.size()) {
        return false;
    }

    // Within index
    this->mCells[row][col] = state;
    this->mCols[col].set(row, state);
    this->mRows[row].set(col, state);
    return true;
}

std::ostream& operator<<(std::ostream& cout, const Grid& grid) {
    std::cout << "\n";
    for (int row = 0; row < (int)grid.mRows.size(); row++) {
        for (int col = 0; col < (int)grid.mCols[0].size(); col++) {
            cout << grid.get(col, row) << " ";
        }
        cout << "\n";
    }
    return cout;
}

