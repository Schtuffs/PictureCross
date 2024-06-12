#pragma once

#include <iostream>
#include <time.h>
#if _WIN32
#include <windows.h>
#undef max
#endif

#define DEFAULT_SIZE    15
#define MAX_RUNTIME     10.0

// For setting squares in grid
#define OPEN            14
#define VALID           10
#define INVALID         12

// For determining complexity
#define SIMPLE          1
#define COMPLEX         2

// Test data, input as is without '// '
// Row Data
// 15 15 1 1 1 1 1 2 2 4 3 1 3 2 6 6 1 4 2 6 8 1 5 3 3 1 8 1 6 2 2 9 1 7 2 1 9 1 8 2 1 9 1 9 2 1 9 1 10 1 8 1 11 1 10 1 12 1 14 1 13 1 15 1 14 1 15 1 15 1 15
// Column Data
// 15 15 2 1 2 8 3 2 2 2 5 4 2 3 2 4 4 2 4 2 3 4 2 5 2 2 5 2 6 2 3 5 2 7 2 4 5 2 8 1 12 2 9 1 12 2 10 1 13 2 11 1 13 2 12 1 15 2 13 1 14 2 14 1 14 2 15 2 8 4
// Combo Data (Use this)
// 15 15 1 1 1 1 1 2 2 4 3 1 3 2 6 6 1 4 2 6 8 1 5 3 3 1 8 1 6 2 2 9 1 7 2 1 9 1 8 2 1 9 1 9 2 1 9 1 10 1 8 1 11 1 10 1 12 1 14 1 13 1 15 1 14 1 15 1 15 1 15 2 1 2 8 3 2 2 2 5 4 2 3 2 4 4 2 4 2 3 4 2 5 2 2 5 2 6 2 3 5 2 7 2 4 5 2 8 1 12 2 9 1 12 2 10 1 13 2 11 1 13 2 12 1 15 2 13 1 14 2 14 1 14 2 15 2 8 4

class PictureCrossGrid {
private:
    int cols, rows;
    bool *completeColumn, *completeRow;
    int **colsHeader, **rowsHeader, **grid;
    int printRowWidth, printColHeight;

    void initColumn(int column);
    void checkColumn(int column);
    bool invalidateColumn(int column);
    void fillColumnSection(int column, int start, int end, int state);

    void initRow(int row);
    void checkRow(int row);
    void invalidateRow(int row);
    void fillRowSection(int row, int start, int end, int state);

    bool checkCompletion();
    void display();
    void clear();
    
public:
    // Defaults to a 15x15 grid
    PictureCrossGrid();

    // Allows for columns and rows to be different values
    PictureCrossGrid(int width, int height);

    // Allows user to input column information
    // User entered headerInfo array pointer will be deleted with this function
    void setColumn(int columnNumber, int* headerInfo, int size);
    int getColumnCount();
    int* getColumn(int column);

    // Allows user to input row information
    // User entered headerInfo array pointer will be deleted with this function
    void setRow(int rowNumber, int* headerInfo, int size);
    int getRowCount();
    int* getRow(int row);

    // Call to solve all currently inputted data
    void solve();

    // Removes memory allocated during column/row sets
    ~PictureCrossGrid();
};

