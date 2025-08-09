#include "Grid.h"

#include "Defines.h"

// ----- Creation ----- Destruction -----

Grid::Grid(const Array<Array<int>>& colInfo, const Array<Array<int>>& rowInfo) : mCells(rowInfo.size()), mCols(colInfo.size()), mRows(rowInfo.size()) {
    for (int i = 0; i < this->mRows.size(); i++) {
        this->mCells[i] = Array<STATE>(this->mCols.size());
    }

    // Add header data
    for (int i = 0; i < this->mCols.size(); i++) {
        this->mCols[i] = Line(this->mRows.size());
        this->mCols[i].head(colInfo[i]);
    }
    for (int i = 0; i < this->mRows.size(); i++) {
        this->mRows[i] = Line(this->mCols.size());
        this->mRows[i].head(rowInfo[i]);
    }
}

Grid::~Grid() {}



// ----- Read -----

const Line& Grid::col(int index) const noexcept {
    if (0 > index || index > this->mCols.size()) {
        return this->mCols[0];
    }

    return this->mCols[index];
}

const Line& Grid::row(int index) const noexcept {
    if (0 > index || index > (int)this->mCells.size()) {
        return this->mRows[0];
    }
    return this->mRows[index];
}

STATE Grid::get(int col, int row) const noexcept {
    if (0 > col || col > this->mCells[0].size()) {
        return STATE::UNKNOWN;
    }
    if (0 > row || row > this->mCells.size()) {
        return STATE::UNKNOWN;
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
            switch (grid.get(col, row)) {
            case STATE::VALID:
                cout << "V";
                break;
            case STATE::INVALID:
                cout << "I";
                break;
            case STATE::NONE:
                cout << "N";
                break;
            default:
                cout << "?";
                break;
            }

            if (col != grid.row(0).size() - 1) {
                cout << ", ";
            }
        }
        cout << "\n";
    }
    return cout;
}

