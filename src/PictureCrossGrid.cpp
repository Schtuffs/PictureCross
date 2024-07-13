#include "../include/PictureCrossGrid.h"

PictureCrossGrid::PictureCrossGrid(int columns, int rows) {
    if (columns <= 0 || rows <= 0) {
        columns = DEFAULT_SIZE;
        rows = DEFAULT_SIZE;
    }
    
    this->cols = columns;
    this->rows = rows;
    
    this->completeColumn = new bool[this->cols];
    this->completeRow = new bool[this->rows];

    // Sets up column pointers
    this->colsHeader = new int*[this->cols];
    for(int i = 0; i < this->cols; i++) {
        this->colsHeader[i] = new int[2];
        this->colsHeader[i][0] = 1;
        this->colsHeader[i][1] = 0;
        this->completeColumn[i] = false;
    }
    

    // Sets up row pointers
    this->rowsHeader = new int*[this->rows];
    for(int i = 0; i < this->rows; i++) {
        this->rowsHeader[i] = new int[2];
        this->rowsHeader[i][0] = 1;
        this->rowsHeader[i][1] = 0;
        this->completeRow[i] = false;
    }
    

    // Sets up grid to open
    this->grid = new int*[this->cols];
    for(int i = 0; i < this->cols; i++) {
        this->grid[i] = new int[this->rows];
        for(int j = 0; j < this->rows; j++) {
            this->grid[i][j] = OPEN;
        }
    }

    std::cout << "Grid created with size of " << cols << "x" << rows << "." << std::endl;
}



void PictureCrossGrid::setColumn(int columnNumber, int* headerInfo, int size) {
    // Reduces index for easier user input
    columnNumber--;
    if (0 > columnNumber || columnNumber > this->cols) {
        return;
    }

    // Deletes old pointer before creating new one
    delete [] this->colsHeader[columnNumber];

    // Creates new pointer and sets first index to size for later use
    this->colsHeader[columnNumber] = new int[size + 1];
    this->colsHeader[columnNumber][0] = size;
    
    // Loops through all indexes of headerinfo, moves them one index down in new array
    for(int i = 1; i <= size; i++) {
        this->colsHeader[columnNumber][i] = headerInfo[i-1];
    }
    delete [] headerInfo;
}

int PictureCrossGrid::getColumnCount() {
    return this->cols;
}

int* PictureCrossGrid::getColumn(int column) {
    column--;
    // Ensures user enters valid column
    if (0 <= column && column < this->cols)
        return this->colsHeader[column];
    return NULL;
}



void PictureCrossGrid::setRow(int rowNumber, int* headerInfo, int size) {
    // Fixes indexing
    rowNumber--;
    if (0 > rowNumber || rowNumber > this->rows) {
        return;
    }

    // Deletes old pointer before creating new one
    delete [] this->rowsHeader[rowNumber];

    // Creates new pointer and sets first index to size for later use
    this->rowsHeader[rowNumber] = new int[size + 1];
    this->rowsHeader[rowNumber][0] = size;
    
    // Loops through all indexes of headerinfo, moves them one index down in new array
    for(int i = 1; i <= size; i++) {
        this->rowsHeader[rowNumber][i] = headerInfo[i-1];
    }
    delete [] headerInfo;
}

int PictureCrossGrid::getRowCount() {
    return this->rows;
}

int* PictureCrossGrid::getRow(int row) {
    row--;
    // Ensures user enters valid row
    if (0 <= row && row < this->rows)
        return this->rowsHeader[row];
    return NULL;
}



void PictureCrossGrid::init(int crNum, int **dataHeader, int maxCR, bool *completeHeader, void (PictureCrossGrid::*fillSection)(int crNum, int startIndex, int totalSquares, int state)) {
    // Stores data from column or row information for easier data use
    int* data = dataHeader[crNum];

    // Takes total cols/rows, accounts for extra spaces between, and calculates total amount of known data
    int remove = maxCR - (data[0] - 1);
    for(int i = 1; i <= data[0]; i++) {
        remove -= data[i];
    }

    if (remove == 0) {
        // Loop through all data points add add them to grid
        int startIndex = 0;
        for(int i = 1; i <= data[0]; i++) {
            // Sets the valid data
            (this->*fillSection)(crNum, startIndex, data[i], VALID);
            // Moves the start index for the invalid square
            startIndex += data[i];

            // Adds the invalid square
            (this->*fillSection)(crNum, startIndex, 1, INVALID);

            // Final update for after invalid square
            startIndex++;
        }

        // Completes dataset and returns
        completeHeader[crNum] = true;
        return;
    }


    // Data doesn't fully fill section

    // Stores index for filling data
    int startIndex = 0;
    for(int i = 1; i <= data[0]; i++) {
        // Number of datapoints to fill
        int valueToFill = data[i];

        // Removes extra points for proper resizing
        valueToFill -= remove;

        // Fill in section data
        (this->*fillSection)(crNum, startIndex + remove, valueToFill, VALID);
            
        // Start index increases by the data value and extra space
        startIndex += data[i] + 1;
    }
}

void PictureCrossGrid::check(int COL_OR_ROW, int crNum, int **dataHeader, int maxCR, bool *completeHeader, void (PictureCrossGrid::*fillSection)(int crNum, int startIndex, int totalSquares, int state)) {
    // Checks first if all squares for col/row have been filled in
    
    if (this->checkTotalSquares(COL_OR_ROW, crNum, dataHeader, maxCR, completeHeader, fillSection)) {
        return;
    }

    // Checks for squares along the edge to fill in outer points
    if (this->checkEdges(COL_OR_ROW, crNum, dataHeader, maxCR, completeHeader, fillSection)) {
        return;
    }

    // Checks for different sub sections
    if (this->checkSubSections(COL_OR_ROW, crNum, dataHeader, maxCR, completeHeader, fillSection)) {
        return;
    }
}

bool PictureCrossGrid::checkTotalSquares(int COL_OR_ROW, int crNum, int **dataHeader, int maxCR, bool *completeHeader, void (PictureCrossGrid::*fillSection)(int crNum, int startIndex, int totalSquares, int state)) {
    // Allows for easy indexing of data
    int *data = dataHeader[crNum];

    // Check how many valid squares are present
    int validSquares = 0;
    for(int i = 0; i < maxCR; i++) {
        if (this->checkGrid(COL_OR_ROW, crNum, i, VALID)) {
            validSquares++;
        }
    }

    // Check how many spaces there should be in total
    int takenSpaces = 0;
    for(int i = 1; i <= data[0]; i++) {
        takenSpaces += data[i];
    }

    // Check if all squares are taken up properly
    if (validSquares == takenSpaces) {
        (this->*fillSection)(crNum, 0, maxCR, INVALID);
        completeHeader[crNum] = true;
        return true;
    }
    return false;
}

bool PictureCrossGrid::checkEdges(int COL_OR_ROW, int crNum, int **dataHeader, int maxCR, bool *completeHeader, void (PictureCrossGrid::*fillSection)(int crNum, int startIndex, int totalSquares, int state)) {
    int *data = dataHeader[crNum];

    // First index check
    int completeData = 0;
    if (this->checkGrid(COL_OR_ROW, crNum, 0, VALID)) {
        
        (this->*fillSection)(crNum, 0, data[1], VALID);
        (this->*fillSection)(crNum, data[1], 1, INVALID);
        completeData++;
    }

    // Last index check
    if (this->checkGrid(COL_OR_ROW, crNum, maxCR - 1, VALID)) {
        int finalIndex = data[data[0]];
        int startIndex = maxCR - finalIndex;
        (this->*fillSection)(crNum, startIndex, finalIndex, VALID);
        (this->*fillSection)(crNum, startIndex - 1, 1, INVALID);
        completeData++;
    }

    // Check if all required data was filled in during this check
    if (completeData == data[0]) {
        (this->*fillSection)(crNum, 0, this->rows, INVALID);
        completeHeader[crNum] = true;
        return true;
    }

    // If no, check if there is an invalid square blocking data from edges
    // First edge check

    // Variable for after this section
    int firstIndex = 0;

    for(int i = firstIndex; i < data[1]; i++) {
        if (this->checkGrid(COL_OR_ROW, crNum, i, INVALID)) {
            (this->*fillSection)(crNum, 0, i, INVALID);
            firstIndex = i + 1;
        }
    }
    
    // Variable for after this section
    int lastIndex = maxCR - 1;

    // Second edge check
    for(int i = lastIndex; i >= maxCR - data[data[0]]; i--) {
        if (this->checkGrid(COL_OR_ROW, crNum, i, INVALID)) {
            (this->*fillSection)(crNum, i, maxCR, INVALID);
            lastIndex = i - 1;
        }
    }

    // Finally, check if first known point is close enough to a wall to add more information

    // First edge
    for(int i = firstIndex; i < firstIndex + data[1]; i++) {
        if (this->checkGrid(COL_OR_ROW, crNum, i, VALID)) {
            int remove = i - firstIndex;
            (this->*fillSection)(crNum, i, data[1] - remove, VALID);
            break;
        }
    }

    // Second edge
    for(int i = lastIndex; i >= lastIndex - data[data[0]]; i--) {
        if (this->checkGrid(COL_OR_ROW, crNum, i, VALID)) {
            int remove = lastIndex - i;
            int spacing = data[data[0]] - remove - 1;
            (this->*fillSection)(crNum, i - spacing, data[data[0]] - remove, VALID);
            break;
        }
    }

    return false;
}

bool PictureCrossGrid::checkSubSections(int COL_OR_ROW, int crNum, int **dataHeader, int maxCR, bool *completeHeader, void (PictureCrossGrid::*fillSection)(int crNum, int startIndex, int totalSquares, int state)) {
    int *data = dataHeader[crNum];
    
    // First, tries to fill in sections that cant have valid squares
    int openSquares = 0, startIndex = 0;
    for(int i = 0; i < maxCR; i++) {
        if (this->checkGrid(COL_OR_ROW, crNum, i, VALID) ||
            this->checkGrid(COL_OR_ROW, crNum, i, OPEN)) {
            
            openSquares++;
        }
        else {
            for (int j = 1; j <= data[0]; j++) {
                if (openSquares >= data[j]) {
                    break;
                }
                else if (j == data[0]) {
                    (this->*fillSection)(crNum, startIndex, i - startIndex, INVALID);
                }
            }
            startIndex = i;
            openSquares = 0;
        }
    }
    
    // Next, count open sections
    int totalSections = 0;
    int holdState = OPEN;
    for(int i = 0; i < this->rows; i++) {
        // Checks for first valid between invalid
        if ((this->checkGrid(COL_OR_ROW, crNum, i, VALID) ||
            this->checkGrid(COL_OR_ROW, crNum, i, OPEN)) && 
            holdState != VALID) {

            totalSections++;
            holdState = VALID;
        } 
        // Checks for invalid square to allow for another section to be sensed
        else if (this->checkGrid(COL_OR_ROW, crNum, i, INVALID) && holdState != INVALID) {
            holdState = INVALID;
        }
    }
    
    // Now, compare open sections to what the sections should be
    
    // Equal amount of sections
    if (totalSections == data[0]) {
        if (this->fillEqualSections(COL_OR_ROW, crNum, dataHeader, maxCR, completeHeader, fillSection)) {
            return true;
        }
    }
    return false;
}

bool PictureCrossGrid::fillEqualSections(int COL_OR_ROW, int crNum, int **dataHeader, int maxCR, bool *completeHeader, void (PictureCrossGrid::*fillSection)(int crNum, int startIndex, int totalSquares, int state)) {
    int* data = dataHeader[crNum];

    // Stores the number of completed sections
    int totalFilledSections = 0;

    int currentIndex = 0;
    for(int i = 1; i <= data[0]; i++) {
        int validSquares = 0, openSquares = 0, startIndex = 0, validStartIndex = -1;
        bool openingFound = false, validFound = false;
        
        while (currentIndex < maxCR) {
            // Finds where the opening begins
            if (!this->checkGrid(COL_OR_ROW, crNum, currentIndex, INVALID) && !openingFound) {
                
                openingFound = true;
                startIndex = currentIndex;
            }

            // Counts all of the open/valid squares
            if (this->checkGrid(COL_OR_ROW, crNum, currentIndex, VALID) && openingFound) {
                validSquares++;
                openSquares++;
                
                // Fills in between valid square sections, otherwise set start index
                if (validStartIndex != -1)
                    (this->*fillSection)(crNum, validStartIndex, currentIndex - validStartIndex, VALID);
                else
                    validStartIndex = currentIndex;
            }
            else if (this->checkGrid(COL_OR_ROW, crNum, currentIndex, OPEN) && openingFound) {
                openSquares++;
            }

            // Leaves once end of opening found
            else if (this->checkGrid(COL_OR_ROW, crNum, currentIndex, INVALID) &&
                    openingFound) {
                break;
            }
            currentIndex++;
        }

        // Determines if size is same as data. All data is already filled in
        if (validSquares == data[i]) {
            (this->*fillSection)(crNum, startIndex - 1, data[i] + 2, INVALID);
            totalFilledSections++;
        }
        // Otherwise fill in as much as possible. Open squares are equal to the data
        else if (openSquares == data[i]) {
            (this->*fillSection)(crNum, startIndex, openSquares, VALID);
            totalFilledSections++;
        }
        // Data is too small to fit in section. Must shrink data down
        else {
            int remove = openSquares - data[i];
            (this->*fillSection)(crNum, startIndex + remove, data[i] - remove, VALID);
        }
    }

    // If all sections were fully filled in, can complete section
    if (totalFilledSections == data[0]) {
        (this->*fillSection)(crNum, 0, maxCR, INVALID);
        completeHeader[crNum] = true;
        return true;
    }

    return false;
}

bool PictureCrossGrid::checkGrid(int COL_OR_ROW, int crNum, int index, int state) {
    if ((this->grid[crNum][index] == state && COL_OR_ROW == COL) ||
        (this->grid[index][crNum] == state && COL_OR_ROW == ROW)) {
        
        return true;
    }
    return false;
}



void PictureCrossGrid::fillColumnSection(int column, int startIndex, int spaces, int state) {
    // Ensures data begins within bounds of the array
    if (startIndex >= this->rows) {
        return;
    }
    
    for(int i = 0; i < spaces; i++) {
        // Ensures that data can't out of index during looping
        if (i + startIndex >= this->rows)
            return;
        
        // Checks index is valid
        if (i + startIndex < 0)
            continue;
        
        // Check that spot isn't already filled in
        if (this->grid[column][i + startIndex] != OPEN)
            continue;
        
        // Sets data index to be proper state
        this->grid[column][i + startIndex] = state;
    }
}

void PictureCrossGrid::fillRowSection(int row, int startIndex, int spaces, int state) {
    // Ensures data begins within bounds of the array
    if (startIndex >= this->cols) {
        return;
    }
    
    for(int i = 0; i < spaces; i++) {
        // Ensures that data can't out of index during looping
        if (i + startIndex >= this->cols) {
            return;
        }

        // Checks index is valid
        if (i + startIndex < 0)
            continue;
        
        // Check that spot isn't already filled in
        if (this->grid[i + startIndex][row] != OPEN)
            continue;
        
        // Sets data index to be proper state
        this->grid[i + startIndex][row] = state;
    }
}



void PictureCrossGrid::quickSolve() {
    clock_t begin = clock();
    double runtime = 0;

    // Initialize all columns then rows
    for(int i = 0; i < this->cols; i++) {
        this->init(i, this->colsHeader, this->rows, this->completeColumn, &PictureCrossGrid::fillColumnSection);
    }
    
    for(int i = 0; i < this->rows; i++) {
        this->init(i, this->rowsHeader, this->cols, this->completeRow, &PictureCrossGrid::fillRowSection);
    }
    
    // Loops until either time limit is hit, or puzzle is solved
    do {
        for(int i = 0; i < this->cols; i++) {
            this->check(COL, i, this->colsHeader, this->rows, this->completeColumn, &PictureCrossGrid::fillColumnSection);
        }

        for(int i = 0; i < this->rows; i++) {
            this->check(ROW, i, this->rowsHeader, this->cols, this->completeRow, &PictureCrossGrid::fillRowSection);
        }

        clock_t current = clock();
        runtime = (double)(current - begin) / (double)CLOCKS_PER_SEC;

    } while(!this->checkCompletion() && runtime < MAX_QUICK_RUNTIME);

    std::cout << std::endl << std::endl << std::endl;
    std::cout << "Total Elapsed time: " << runtime << "s" << std::endl;
    
    this->display();
    this->clear();
}

void PictureCrossGrid::bruteSolve() {
    std::cout << "This may take a while... (Allow up to " << MAX_BRUTE_RUNTIME << "s)" << std::endl;
    std::cout << "Do not expect this to solve anything above an 5x5 grid." << std::endl;

    clock_t begin = clock();
    double runtime = 0;

    // Simply does a binary-like progression through the whole grid
    int maxIndex = 0;
    do {
        // Loops until maxIndex is reached, instead of recursive function
        for(int j = 0; j <= maxIndex; j++) {
            int col = j % this->cols;
            int row = j / this->cols;
            
            if (this->grid[col][row] == VALID) {
                this->grid[col][row] = INVALID;
            }
            else {
                this->grid[col][row] = VALID;
                break;
            }
            
            if (j == maxIndex) {
                maxIndex++;
            }
        }

        // // Does the check for datapoints
        // for(int i = 0; i < this->cols; i++) {
        //     if (!this->invalidateColumn(i)) {
        //         this->completeColumn[i] = false;
        //         goto bruteEnd;
        //     }
        // }
        // for(int i = 0; i < this->rows; i++){
        //     if (!this->invalidateRow(i)) {
        //         this->completeRow[i] = false;
        //         goto bruteEnd;
        //     }
        // }

    bruteEnd:
        clock_t current = clock();
        runtime = (double)(current - begin) / (double)CLOCKS_PER_SEC;
    } while (!this->checkCompletion() && runtime < MAX_BRUTE_RUNTIME);

    if (this->checkCompletion()) {
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "Total Elapsed time: " << runtime << "s" << std::endl;
        this->display();
    }
    else {
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "Could not solve grid..." << std::endl;
        this->display();
    }
    this->clear();
}



bool PictureCrossGrid::checkCompletion() {
    for(int i = 0; i < this->rows; i++) {
        if (!this->completeRow[i]) {
            return false;
        }
    }
    for(int i = 0; i < this->cols; i++) {
        if (!this->completeColumn[i]) {
            return false;
        }
    }
    return true;
}

void PictureCrossGrid::display() {
#if _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, VALID);
    std::cout << "Select squares with this colour." << std::endl;
    SetConsoleTextAttribute(hConsole, INVALID);
    std::cout << "Do not select squares with this colour." << std::endl;
    SetConsoleTextAttribute(hConsole, OPEN);
    std::cout << "These squares are unclassified." << std::endl;
#elif __linux__
    std::cout << "X: Valid" << std::endl;
    std::cout << "O: Invalid" << std::endl;
    std::cout << "?: Unclassified" << std::endl;
#endif

    for(int i = 0; i < this->rows; i++) {
        for(int j = 0; j < this->cols; j++) {
#if _WIN32
            SetConsoleTextAttribute(hConsole, grid[j][i]);
            std::cout << "X ";
#elif __linux__
            if (grid[j][i] == VALID) {
                std::cout << "X ";
            } else if (grid[j][i] == INVALID) {
                std::cout << "O ";
            } else {
                std::cout << "? ";
            }
#endif
        }
        std::cout << std::endl;
    }
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, 15);
#endif
}

void PictureCrossGrid::clear() {
    // Clears the grid
    for(int i = 0; i < this->cols; i++) {
        for(int j = 0; j < this->rows; j++) {
            grid[i][j] = OPEN;
        }
    }
    
    // Clears the completed columns
    for(int i = 0; i < this->cols; i++) {
        this->completeColumn[i] = false;
    }

    // Clears the completed rows
    for(int i = 0; i < this->rows; i++) {
        this->completeRow[i] = false;
    }
}

PictureCrossGrid::~PictureCrossGrid() {
    for(int i = 0; i < this->cols; i++) {
        delete [] this->colsHeader[i];
    }
    delete [] this->colsHeader;

    for(int i = 0; i < this->rows; i++) {
        delete [] this->rowsHeader[i];
    }
    delete [] this->rowsHeader;

    for(int i = 0; i < this->cols; i++) {
        delete [] this->grid[i];
    }
    delete [] this->grid;
    std::cout << "Deletion Complete!" << std::endl;
}

