#include "../include/PictureCrossGrid.h"

PictureCrossGrid::PictureCrossGrid() {
    this->cols = DEFAULT_SIZE;
    this->rows = DEFAULT_SIZE;

    this->colsHeader = new int*[this->cols];
    for(int i = 0; i < this->cols; i++) {
        this->colsHeader[i] = new int[1];
        this->colsHeader[i][0] = 0;
    }

    this->rowsHeader = new int*[this->rows];
    for(int i = 0; i < this->rows; i++) {
        this->rowsHeader[i] = new int[1];
        this->rowsHeader[i][0] = 0;
    }

    this->grid = new int*[this->cols];
    for(int i = 0; i < this->cols; i++) {
        this->grid[i] = new int[this->rows];
        for(int j = 0; j < this->rows; j++) {
            this->grid[i][j] = OPEN;
        }
    }
    std::cout << "Grid created with size of " << cols << "x" << rows << "." << std::endl;
}

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



void PictureCrossGrid::initColumn(int column) {
    // Stores the array information for this column
    // Easier for indexing later
    int* data = this->colsHeader[column];

    // Take total rows, account for the total spaces (removing the extra number for spaces), then subtract all of the numbers
    int remove = this->rows - (data[0] - 1);
    for(int i = 1; i <= data[0]; i++) {
        remove -= data[i];
    }

    // If the numbers are able to fill column, fill each cell

    if (remove == 0) {
        // Loop through all data points add add them to column
        int startIndex = 0;
        for(int i = 1; i <= data[0]; i++) {
            // Sets the valid data
            this->fillColumnSection(column, startIndex, data[i], VALID);

            // Moves the start index for the invalid square
            startIndex += data[i];

            // Adds the invalid square
            this->fillColumnSection(column, startIndex, 1, INVALID);

            // Final update for after invalid square
            startIndex++;
        }
        // To prevent everything in another else statement
        return;
    }

    // Data isn't equal to total column value

    // Starts indexes at first guarenteed data location
    int startIndex = remove;
    for(int i = 1; i <= data[0]; i++) {
        // Value of data to fill
        int valueToFill = data[i];

        // Removes length from this section
        valueToFill -= remove;

        this->fillColumnSection(column, startIndex, valueToFill, VALID);
        // Start index increases by half of removed(rounded up) and by the data value
        startIndex += ((remove + 1) / 2) + data[i];
    }
}

void PictureCrossGrid::checkColumn(int column) {
    // Check if this columns data is already complete
    if (this->completeColumn[column]) {
        return;
    }

    // First, invalidate column if applicable. If the rest of the column is invalidated, then return
    if (this->invalidateColumn(column)) {
        return;
    }

    int* data = this->colsHeader[column];
    
    // See if first number can fit at top
    int startIndex = 0, totalSpaces = 0, currentValueCheck = 1;
    for(int i = 0; i < this->rows; i++) {
        // Once invalid is found, reset spaces
        if (this->grid[column][i] == INVALID) {
            // If space isn't large enough, fill with invalid
            if (totalSpaces < data[currentValueCheck]) {
                this->fillColumnSection(column, startIndex, totalSpaces, INVALID);
                startIndex = totalSpaces;
                totalSpaces = 0;
            }
        }

        totalSpaces++;

        // Check if slot is big enough
    }
    
    if (this->checkTopBottom(column)) {
        return;
    }

    // Otherwise check how much info can be filled still
    
    // See if sections have been solved
    int totalSections = 0;
    int holdState = OPEN;
    for(int i = 0; i < this->rows; i++) {
        // Checks for first valid between invalid
        if (this->grid[column][i] == VALID && holdState != VALID) {
            totalSections++;
            holdState = VALID;
        } 
        // Checks for invalid square to allow for another section to be sensed
        else if (this->grid[column][i] == INVALID && holdState != INVALID) {
            holdState = INVALID;
        }
    }

    // If total sections are equal to the data sections
    if (totalSections == data[0]) {
        this->fillKnownSubColumn(column);
        return;
    }
    // Otherwise fill in guarenteed spots
    this->fillUnknownSubColumn(column);
}

bool PictureCrossGrid::invalidateColumn(int column) {
    int* data = this->colsHeader[column];

    // Checks how many valid squares are present
    int valid = 0;
    for(int i = 0; i < this->rows; i++) {
        if (this->grid[column][i] == VALID) {
            valid++;
        }
    }

    // Determine how many spaces should be taken up
    int takenSpaces = 0;
    for(int i = 1; i <= data[0]; i++) {
        takenSpaces += data[i];
    }

    // Compare values, if equal, rest of column should be invalid
    if (takenSpaces == valid) {
        this->fillColumnSection(column, 0, this->rows, INVALID);
        this->completeColumn[column] = true;
        return true;
    }
    return false;
}

bool PictureCrossGrid::checkTopBottom(int column) {
    int* data = this->colsHeader[column];

    // Check if first number is complete
    int spaces = 0;

    // Top check
    int completeData = 0;
    if (this->grid[column][0] == VALID) {
        this->fillColumnSection(column, 0, data[1], VALID);
        this->fillColumnSection(column, data[1], 1, INVALID);
        completeData++;
    }
    // Bottom check
    if (this->grid[column][this->rows - 1] == VALID) {
        int finalIndex = data[data[0]];
        int startIndex = this->rows - finalIndex;
        this->fillColumnSection(column, startIndex, finalIndex, VALID);
        this->fillColumnSection(column, startIndex - 1, 1, INVALID);
        completeData++;
    }

    // Check if all required data was filled in during this check
    if (completeData == data[0]) {
        this->fillColumnSection(column, 0, this->rows, INVALID);
        this->completeColumn[column] = true;
        return true;
    }
    return false;
}

void PictureCrossGrid::fillKnownSubColumn(int column) {
    int* data = this->colsHeader[column];

    // Stores the actual starting point for the next iteration
    int nextStartIndex = 0;

    // Prevents infinite loops
    int totalIterations = 0;

    // Main loop for going through each data point
    for(int i = 1; i <= data[0] && totalIterations < this->rows; i++) {
        // Counts the valid squares in a section
        int validSquares = 0;

        // Gets moved along as loop goes further
        int startIndex = nextStartIndex;

        // Stores if section has found a valid square
        bool isValidFound = false;
        
        // Loops through the next part of the column to find the proper gap
        for(int j = nextStartIndex; j < this->rows; j++) {
            // Moves pointer for next time through loop
            nextStartIndex++;

            // If a valid point is found, mark it
            if (this->grid[column][j] == VALID)
                isValidFound = true;

            // Finds an opening
            if (this->grid[column][j] != INVALID) {
                validSquares++;
            }
            // Invalid found
            else {
                break;
            }
        }

        // See if box is the exact size
        if (isValidFound && validSquares == data[i]) {
            this->fillColumnSection(column, startIndex, data[i], VALID);
        }

        // See if the spot is big enough
        else if (isValidFound && validSquares >= data[i]) {
            int remove = validSquares - data[i];
            this->fillColumnSection(column, startIndex + remove, data[i] - (remove * 2), VALID);
        }
        // Slot either wasn't big enough, or valid square wasn't found
        else {
            i--; // Lets this index continue to try to find valid opening
        }
        totalIterations++;
    }
}

void PictureCrossGrid::fillColumnSection(int column, int startIndex, int spaces, int state) {
    // Ensures data begins within bounds of the array
    if (0 > startIndex || startIndex > this->rows)
        return;
    
    for(int i = 0; i < spaces; i++) {
        // Ensures that data can't out of index during looping
        if (i + startIndex > this->rows)
            return;
        
        // Check that spot isn't already filled in
        if (this->grid[column][i + startIndex] != OPEN) {
            continue;
        }
        
        // Sets data index to be proper state
        this->grid[column][i + startIndex] = state;
    }
}



void PictureCrossGrid::initRow(int row) {
    // Stores the array information for this row
    // Easier for indexing later
    int* data = this->rowsHeader[row];

    // Take total columns, account for the total spaces (removing the extra number for spaces), then subtract all of the numbers
    int remove = this->cols - (data[0] - 1);
    for(int i = 1; i <= data[0]; i++) {
        remove -= data[i];
    }

    // If the numbers are able to fill row, fill each cell
    if (remove == 0) {
        // Loop through all data points add add them to row
        int startIndex = 0;
        for(int i = 1; i <= data[0]; i++) {
            // Sets the valid data
            this->fillRowSection(row, startIndex, data[i], VALID);

            // Moves the start index for the invalid square
            startIndex += data[i];

            // Adds the invalid square after dataset
            if (i != data[0])
                this->fillRowSection(row, startIndex, 1, INVALID);

            // Final update for after invalid square
            startIndex++;
        }
        // Final filling of all spaces
        this->fillRowSection(row, 0, this->cols, INVALID);

        // To prevent everything in another else statement
        return;
    }

    // Data isn't equal to total row value

    // See how much data is known from edge points of grid
    // Left check
    this->checkLeftRight(row);

    // Starts indexes at first guarenteed data location
    int startIndex = remove;
    for(int i = 1; i <= data[0]; i++) {
        // Value of data to fill
        int valueToFill = data[i];

        // Removes length from this section
        valueToFill -= remove;

        this->fillRowSection(row, startIndex, valueToFill, VALID);
        // Start index increases by half of removed(rounded up) and by the data value
        startIndex += ((remove + 1) / 2) + data[i];
    }
}

void PictureCrossGrid::checkRow(int row) {
    // Check if this columns data is already complete
    if (this->completeRow[row]) {
        return;
    }

    // First, invalidate column if applicable. If the rest of the column is invalidated, then return
    if (this->invalidateRow(row)) {
        return;
    }

    int* data = this->rowsHeader[row];
    
    // See if first number can fit at top
    int startIndex = 0, totalSpaces = 0, currentValueCheck = 1;
    for(int i = 0; i < this->cols; i++) {
        // Once invalid is found, reset spaces
        if (this->grid[i][row] == INVALID) {
            // If space isn't large enough, fill with invalid
            if (totalSpaces < data[currentValueCheck]) {
                this->fillRowSection(row, startIndex, totalSpaces, INVALID);
                startIndex = totalSpaces;
                totalSpaces = 0;
            }
        }
        totalSpaces++;

    // Check if slot is big enough
    }
    if (this->checkLeftRight(row)) {
        return;
    }

    // Otherwise check how much info can be filled still
    
    // See if sections have been solved
    int totalSections = 0;
    int holdState = 0;
    for(int i = 0; i < this->cols; i++) {
        // Checks for first valid between invalid
        if (this->grid[i][row] == VALID && holdState != VALID) {
            totalSections++;
            holdState = VALID;
        } 
        // Checks for invalid square to allow for another section to be sensed
        else if (this->grid[i][row] == INVALID && holdState != INVALID) {
            holdState = INVALID;
        }
    }

    if (totalSections == data[0]) {
        this->fillKnownSubRow(row);
        return;
    }
}

bool PictureCrossGrid::invalidateRow(int row) {
    int* data = this->rowsHeader[row];

    // Checks how many valid squares are present
    int valid = 0;
    for(int i = 0; i < this->cols; i++) {
        if (this->grid[i][row] == VALID) {
            valid++;
        }
    }

    // Determine how many spaces should be taken up
    int takenSpaces = 0;
    for(int i = 1; i <= data[0]; i++) {
        takenSpaces += data[i];
    }

    // Compare values, if equal, rest of row should be invalid
    if (takenSpaces == valid) {
        this->fillRowSection(row, 0, this->cols, INVALID);
        this->completeRow[row] = true;
        return true;
    }
    return false;
}

bool PictureCrossGrid::checkLeftRight(int row) {
    int* data = this->rowsHeader[row];
    
    // Left check
    int completeData = 0;
    if (this->grid[0][row] == VALID) {
        this->fillRowSection(row, 0, data[1], VALID);
        this->fillRowSection(row, data[1], 1, INVALID);
        completeData++;
    }
    // Right check
    if (this->grid[this->cols - 1][row] == VALID) {
        // Gets the final info point from data
        int finalIndex = data[data[0]];
        int startIndex = this->cols - finalIndex;
        this->fillRowSection(row, startIndex, finalIndex, VALID);
        this->fillRowSection(row, startIndex-1, 1, INVALID);
        completeData++;
    }

    // Check if all required data was filled in during this check
    if (completeData == data[0]) {
        this->fillRowSection(row, 0, this->cols, INVALID);
        this->completeRow[row] = true;
        return true;
    }
    return false;
}

void PictureCrossGrid::fillKnownSubRow(int row) {
    int* data = this->rowsHeader[row];
   
    // Stores the actual starting point for the next iteration
    int nextStartIndex = 0;

    // Prevents infinite loops
    int totalIterations = 0;

    // Main loop for going through each data point
    for(int i = 1; i <= data[0] && totalIterations < this->cols; i++) {
        // Counts the valid squares in a section
        int validSquares = 0;

        // Gets moved along as loop goes further
        int startIndex = nextStartIndex;

        // Stores if section has found a valid square
        bool isValidFound = false;
        
        // Loops through the next part of the row to find the proper gap
        for(int j = nextStartIndex; j < this->cols; j++) {
            // Moves pointer for next time through loop
            nextStartIndex++;

            // If a valid point is found, mark it
            if (this->grid[j][row] == VALID)
                isValidFound = true;

            // Finds an opening
            if (this->grid[j][row] != INVALID) {
                validSquares++;
            }
            // Invalid found
            else {
                break;
            }
        }

        // See if box is the exact size
        if (isValidFound && validSquares == data[i]) {
            this->fillRowSection(row, startIndex, data[i], VALID);
        }

        // See if the spot is big enough
        else if (isValidFound && validSquares >= data[i]) {
            int remove = validSquares - data[i];
            this->fillRowSection(row, startIndex + remove, data[i] - (remove * 2), VALID);
        }
        // Slot either wasn't big enough, or valid square wasn't found
        else {
            i--; // Lets this index continue to try to find valid opening
        }
        totalIterations++;
    }
}

void PictureCrossGrid::fillRowSection(int row, int startIndex, int spaces, int state) {
    // Ensures data begins within bounds of the array
    if (0 > startIndex || startIndex > this->cols)
        return;
    
    for(int i = 0; i < spaces; i++) {
        // Ensures that data can't out of index during looping
        if (i + startIndex > this->cols)
            return;
        
        // Check that spot isn't already filled in
        if (this->grid[i + startIndex][row] != OPEN) {
            continue;
        }
        
        // Sets data index to be proper state
        this->grid[i + startIndex][row] = state;
    }
}



void PictureCrossGrid::quickSolve() {
    clock_t begin = clock();
    double runtime = 0;

    // Initialize all columns then rows
    for(int i = 0; i < this->cols; i++) {
        this->initColumn(i);
    }
    for(int i = 0; i < this->rows; i++) {
        this->initRow(i);
    }
    
    do {
        for(int i = 0; i < this->cols; i++) {
            this->checkColumn(i);
        }
        for(int i = 0; i < this->rows; i++) {
            this->checkRow(i);
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

        // Does the check for datapoints
        for(int i = 0; i < this->cols; i++) {
            if (!this->invalidateColumn(i)) {
                this->completeColumn[i] = false;
            }
        }
        for(int i = 0; i < this->rows; i++){
            if (!this->invalidateRow(i)) {
                this->completeRow[i] = false;
            }
        }

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

