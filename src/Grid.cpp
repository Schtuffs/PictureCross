#include "Grid.h"

#include "Defines.h"

// ----- Creation ----- Destruction -----

Grid::Grid(int cols, int rows) : mCells(rows) {
    for (int i = 0; i < rows; i++) {
        this->mCells[i] = Array<STATE>(cols);
    }
}

Grid::~Grid() {}



// ----- Setters -----

bool Grid::set(int col, int row, STATE state) noexcept {
    // Check cols
    if (0 > col || col >= this->mCells[0].size()) {
        return false;
    }
    // Check rows
    if (0 > row || row >= this->mCells.size()) {
        return false;
    }

    // Set state
    this->mCells[row][col] = state;
    return true;
}



// ----- Getters -----

const Array<STATE> Grid::col(int index) const noexcept {
    if (index < 0 || index > (int)this->mCells[0].size()) {
        return Array<STATE>();
    }

    // Loop and create the array
    Array<STATE> toReturn(this->mCells[0].size());
    for (int i = 0; i < this->mCells[0].size(); i++) {
        toReturn[i] = this->mCells[i][index];
    }
    return toReturn;
}

const Array<STATE>& Grid::row(int index) const noexcept {
    if (index < 0 || index > (int)this->mCells[0].size()) {
        return this->mCells[0];
    }
    return this->mCells[index];
}

STATE Grid::get(int col, int row) const noexcept {
    if (0 > col || col > this->mCells.size()) {
        return UNKNOWN;
    }
    if (0 > row || row > this->mCells[0].size()) {
        return UNKNOWN;
    }
    return this->mCells[row][col];
}

std::ostream& operator<<(std::ostream& cout, const Grid& grid) {
    std::cout << "\n";
    for (int row = 0; row < (int)grid.mCells.size(); row++) {
        for (int col = 0; col < (int)grid.mCells[0].size(); col++) {
            cout << grid.mCells[row][col] << " ";
        }
        cout << "\n";
    }
    return cout;
}

