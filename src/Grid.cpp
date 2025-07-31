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

bool Grid::set(int col, int row, STATE state) {
    // Check cols
    if (0 > col || col > (int)this->mCells.size()) {
        return false;
    }
    // Check rows
    if (0 > row || row > (int)this->mCells[0].size()) {
        return false;
    }

    // Set state
    this->mCells[row][col] = state;
    return true;
}



// ----- Getters -----

// const Array<STATE>& Grid::cols(int index) const noexcept {
//     if (index < 0 || index > (int)this->mCells.size()) {
//         return Array<STATE>();
//     }
//     return this->mCells[index];
// }

const Array<STATE>& Grid::row(int index) const noexcept {
    if (index < 0 || index > (int)this->mCells[0].size()) {
        return this->mCells[0];
    }
    return this->mCells[index];
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

