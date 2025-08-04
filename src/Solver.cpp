#include "Solver.h"

#include "Defines.h"

// ----- Creation ----- Destruction -----

Solver::Solver(const HeaderInfo& info) : mInfo(info), mGrid(info.col(), info.row()), mSolvedLines(0), mColCount(info.col()), mRowCount(info.row()), mColSize(mInfo.row()), mRowSize(mInfo.col()) {}
Solver::~Solver() {}



// ----- Read -----

double Solver::runtime() const noexcept {
    return this->mRuntime;
}



// ----- Read ----- Private -----

bool Solver::isComplete() {
    return (this->mSolvedLines == (this->mColSize + this->mRowSize));
}

bool Solver::isComplete(const Line& line) const noexcept {
    return true;
}

inline int Solver::lineSections(const Line& line) const noexcept {
    return line.size();
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
        for (int i = 0; i < this->mColCount; i++) {
            this->check(COL, this->mGrid.col(i));
        }

        // Check rows
        for (int i = 0; i < this->mRowCount; i++) {

        }

        // Runtime check
        clock_t current = clock();
        this->mRuntime = (double)(current - begin) / (double)CLOCKS_PER_SEC;
    }
    return this->mGrid;
}



// ----- Update ----- Private -----

// ----- Initializing -----

// Calculates the remaining number of squares in the initial header data
inline int calcRemaining(const Line& line) noexcept {
    // Takes total cols/rows, accounts for extra spaces between, and calculates total amount of known data
    int remove = line.size() - (line.head().size() - 1);
    for (int i = 0; i < line.size(); i++) {
        remove -= line.head()[i];
    }
    return remove;
}

void Solver::initGrid() {
    // Col data
    {
        // Loop through cols
        for (int lineNum = 0; lineNum < this->mColCount; lineNum++) {
            auto curCol = this->mGrid.col(lineNum);
            int remain = calcRemaining(this->mGrid.col(lineNum));

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
        for (int lineNum = 0; lineNum < this->mRowCount; lineNum++) {
            auto curRow = this->mGrid.row(lineNum);
            int remain = calcRemaining(this->mGrid.row(lineNum));
    
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

inline void Solver::initCompleteLine(TYPE type, int lineNum, const Line& line) noexcept {
    // Loop through all data points and add them to grid
    int startIndex = 0;
    for(int i = 0; i < line.size(); i++) {
        // Sets the valid data
        this->fill(type, lineNum, startIndex, line.head()[i], STATE::VALID);

        // Moves the start index for the invalid square
        startIndex += line.head()[i];

        // Adds the invalid square
        this->fill(type, lineNum, startIndex, 1, STATE::INVALID);

        // Final update for after invalid square
        startIndex++;
    }
}

inline void Solver::initIncompleteLine(TYPE type, int lineNum, int remain, const Line& line) noexcept {
    // Stores index for filling data
    int startIndex = 0;
    for(int i = 0; i < line.size(); i++) {
        // Number of datapoints to fill
        int valueToFill = line.head()[i];

        // Removes extra points for proper resizing
        valueToFill -= remain;

        // Fill in section data
        this->fill(type, lineNum, startIndex + remain, valueToFill, STATE::VALID);

        // Start index increases by the data value and extra space
        startIndex += line.head()[i] + 1;
    }
}

// ----- Looping -----

void Solver::check(TYPE type, const Line& line) {
    // See if complete
    if (this->isComplete(line)) {
        return;
    }

    // Check sections
    int sections = this->lineSections((type == COL ? this->mGrid.col(0) : this->mGrid.row(0)));
    if (sections == line.size()) {
        // Split into smaller lines

    }
}

// ----- Other -----

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

