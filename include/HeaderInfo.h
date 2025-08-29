#pragma once

#include "Array.h"

// Data class
// Holds the header information for solving later
class HeaderInfo {
private:
    // Own array class for keeping the array size easy to access
    Array<Array<int>> mDataCol, mDataRow;

public:
    HeaderInfo(int cols, int rows);

    // Sets the values for a given column
    // Uses values 1-size
    bool setCol(int col, const Array<int>& vals);
    // Sets the values for a given row
    // Uses values 1-size
    bool setRow(int row, const Array<int>& vals);

    // Returns the number of columns
    int col() const;
    // Returns the number of rows
    int row() const;

    // Returns all column data
    const Array<Array<int>>& allCols() const;
    // Returns all row data
    const Array<Array<int>>& allRows() const;

    // Returns specified column data
    // Returns empty vector if out of bounds
    const Array<int>& col(int index) const;
    // Returns specified row data
    // Returns empty vector if out of bounds
    const Array<int>& row(int index) const;
    
    ~HeaderInfo();
};
