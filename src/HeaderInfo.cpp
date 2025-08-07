#include "HeaderInfo.h"

#include <iostream>

// ----- Creation ----- Destruction -----

HeaderInfo::HeaderInfo(int cols, int rows) : mDataCol(cols), mDataRow(rows) {
    std::cout << "Initializing grid of size " << cols << "x" << rows << "\n\n";
}

HeaderInfo::~HeaderInfo() {
    // Nothing todo
}



// ----- Read -----

int HeaderInfo::col() const noexcept {
    return this->mDataCol.size();
}

int HeaderInfo::row() const noexcept {
    return this->mDataRow.size();
}

const Array<Array<int>>& HeaderInfo::allCols() const noexcept {
    return this->mDataCol;
}

const Array<Array<int>>& HeaderInfo::allRows() const noexcept {
    return this->mDataRow;
}

const Array<int>& HeaderInfo::col(int index) const noexcept {
    if (index < 0 || index > (int)this->mDataCol.size()) {
        return this->mDataCol[0];
    }
    return this->mDataCol[index];
}

const Array<int>& HeaderInfo::row(int index) const noexcept {
    if (index < 0 || index > (int)this->mDataRow.size()) {
        return this->mDataRow[0];
    }
    return this->mDataRow[index];
}



// ----- Update -----

bool HeaderInfo::setCol(int col, const Array<int>& vals) {
    // Decrease for indexing
    col--;
    if (col < 0 || col >= (int)this->mDataCol.size()) {
        return false;
    }

    this->mDataCol[col] = vals;
    return true;
}

bool HeaderInfo::setRow(int row, const Array<int>& vals) {
    // Decrease for indexing
    row--;
    if (row < 0 || row >= (int)this->mDataRow.size()) {
        return false;
    }
    
    this->mDataRow[row] = vals;
    return true;
}

