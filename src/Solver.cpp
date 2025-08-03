#include "Solver.h"

#include "Defines.h"

// ----- Creation ----- Destruction -----

Solver::Solver(const HeaderInfo& info) : mInfo(info), mGrid(info.col(), info.row()), mSolvedLines(0) {}
Solver::~Solver() {}



// ----- Read -----

double Solver::runtime() const noexcept {
    return this->mRuntime;
}



// ----- Read ----- Private -----

bool Solver::isComplete() {
    return (this->mSolvedLines == (this->mInfo.col() + this->mInfo.row()));
}



// ----- Update -----

const Grid& Solver::solve() {
    // Initial data fill ins
    this->initGrid();

    // Main loop for filling new points while not completed
    clock_t begin = clock();
    this->mRuntime = 0;
    while (!this->isComplete() && this->mRuntime < MAX_RUNTIME) {
        // Check columns
        for (int i = 0; i < this->mInfo.col(); i++) {
            
        }

        // Check rows
        for (int i = 0; i < this->mInfo.row(); i++) {

        }

        // Runtime check
        clock_t current = clock();
        this->mRuntime = (double)(current - begin) / (double)CLOCKS_PER_SEC;
    }
    return this->mGrid;
}

// ----- Update ----- Private -----

// Calculates the remaining number of squares in the initial header data
inline int calcRemaining(int totalLineLength, int lineSize, const Array<int>& line) noexcept {
    // Takes total cols/rows, accounts for extra spaces between, and calculates total amount of known data
    int remove = totalLineLength - (lineSize - 1);
    for (int i = 0; i < lineSize; i++) {
        remove -= line[i];
    }
    return remove;
}

// Fill in the completed line
inline void Solver::initCompleteLine(TYPE type, int lineNum, const Array<int>& line) noexcept {
    // Loop through all data points and add them to grid
    int startIndex = 0;
    for(int i = 0; i < line.size(); i++) {
        // Sets the valid data
        this->fill(type, lineNum, startIndex, line[i], STATE::VALID);

        // Moves the start index for the invalid square
        startIndex += line[i];

        // Adds the invalid square
        this->fill(type, lineNum, startIndex, 1, STATE::INVALID);

        // Final update for after invalid square
        startIndex++;
    }
}

inline void Solver::initIncompleteLine(TYPE type, int lineNum, int remain, const Array<int>& line) noexcept {
    // Stores index for filling data
    int startIndex = 0;
    for(int i = 0; i < line.size(); i++) {
        // Number of datapoints to fill
        int valueToFill = line[i];

        // Removes extra points for proper resizing
        valueToFill -= remain;

        // Fill in section data
        this->fill(type, lineNum, startIndex + remain, valueToFill, STATE::VALID);

        // Start index increases by the data value and extra space
        startIndex += line[i] + 1;
    }
}

void Solver::initGrid() {
    // Col data
    {
        // Loop through cols
        int colTotal = this->mInfo.col();
        int colSize = this->mInfo.row();
        for (int lineNum = 0; lineNum < colTotal; lineNum++) {
            auto curCol = this->mInfo.col(lineNum);
            int remain = calcRemaining(colSize, curCol.size(), this->mInfo.col(lineNum));

            // All squares should be filled
            if (remain == 0) {
                // Fill in data
                this->initCompleteLine(COL, lineNum, curCol);

                // Completes dataset
                this->mSolvedLines++;
            }
            else {
                // Data doesn't fully fill section
                this->initIncompleteLine(COL, lineNum, remain, curCol);
            }
        }
    }
    // Row data
    {
        // Loop through cols
        int rowTotal = this->mInfo.row();
        int rowSize = this->mInfo.col();
        for (int lineNum = 0; lineNum < rowTotal; lineNum++) {
            auto curRow = this->mInfo.row(lineNum);
            int remain = calcRemaining(rowSize, curRow.size(), this->mInfo.row(lineNum));
    
            // All squares should be filled
            if (remain == 0) {
                // Fill in data
                this->initCompleteLine(ROW, lineNum, curRow);
    
                // Completes dataset
                this->mSolvedLines++;
            }
            else {
                // Data doesn't fully fill section
                this->initIncompleteLine(ROW, lineNum, remain, curRow);
            }
        }
    }
}

void Solver::fill(TYPE t, int line, int start, int spaces, STATE state) {
    if (t == COL) {
        // Ensures data begins within bounds of the array
        if (start >= this->mInfo.row()) {
            return;
        }
        
        for(int i = 0; i < spaces; i++) {
            // Ensures that data can't out of index during looping
            if (i + start >= this->mInfo.row()) {
                return;
            }

            // Checks index is valid
            if (i + start < 0) {
                continue;
            }
            
            // Check that spot isn't already filled in
            if (this->mGrid.get(line, i + start) != STATE::NONE) {
                continue;
            }

            // Sets data index to be proper state
            this->mGrid.set(line, i + start, state);
        }
        return;
    }
    else if (t == ROW) {
        // Ensures data begins within bounds of the array
        if (start >= this->mInfo.col()) {
            return;
        }

        for(int i = 0; i < spaces; i++) {
            // Ensures that data can't out of index during looping
            if (i + start >= this->mInfo.col())
                return;

            // Checks index is valid
            if (i + start < 0)
                continue;

            // Check that spot isn't already filled in
            if (this->mGrid.get(i + start, line) != STATE::NONE) {
                continue;
            }

            // Sets data index to be proper state
            this->mGrid.set(i + start, line, state);
        }
        return;
    }
}

