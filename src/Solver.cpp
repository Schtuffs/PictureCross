#include "Solver.h"

#include "Defines.h"
#include "Timer.h"

// ----- Creation ----- Destruction -----

Solver::Solver(const HeaderInfo& info) : mInfo(info), mGrid(info.allCols(), info.allRows()), mSolvedLines(0), mColCount(info.col()), mRowCount(info.row()), mColSize(mInfo.row()), mRowSize(mInfo.col()) {}
Solver::~Solver() {}



// ----- Read -----

double Solver::runtime() const noexcept {
    return this->mRuntime;
}



// ----- Read ----- Private -----

bool Solver::isComplete() {
    return (this->mSolvedLines == (this->mColSize + this->mRowSize));
}

// TODO - CHANGE TO BE QUICKER
bool Solver::isComplete(const Line& line) const noexcept {
    // Get valid squares
    int valid = 0;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == STATE::VALID) {
            valid++;
        }
    }

    // Get head data
    int exp = 0;
    auto head = line.head();
    for (int i = 0; i < head.size(); i++) {
        exp += head[i];
    }

    // The check
    return (valid == exp);
}

inline int Solver::lineSections(const Line& line) const noexcept {
    // Start with first index
    bool inSection = (line[0] == STATE::INVALID ? false : true);

    // Loop through rest of line
    int sections = (inSection ? 1 : 0);
    for (int i = 1; i < line.size(); i++) {
        // Find end section
        if (inSection && line[i] == STATE::INVALID) {
            inSection = false;
        }
        // Find new section
        if (!inSection && line[i] != STATE::INVALID) {
            inSection = true;
            sections++;
        }
    }

    return sections;
}

Array<Line> Solver::lineSplit(const Line& line) const noexcept {
    // Create the array
    Array<Line> lines(line.sections());

    // Update the array with sublines
    int searchIndex = 0, start = 0;
    for (int i = 0; i < lines.size(); i++) {
        // Find the size of the line
        int size = 0;
        while (searchIndex < line.size()) {
            if (line[searchIndex++] != STATE::INVALID) {
                size++;
            }
            else {
                // Ensure line has a size, otherwise started on bad square
                if (size == 0) {
                    start = searchIndex;
                    continue;
                }
                break;
            }
        }
        Array<int> head(1);
        head[0] = line.head()[i];
        lines[i] = Line(size, head);
        lines[i].start(searchIndex - size - 1);
        start = searchIndex;
    }

    return lines;
}



// ----- Update -----

const Grid& Solver::solve() {
    std::cout << "\nBeginning solve...\n";
    
    // Begin solve time
    clock_t begin = clock();
    this->mRuntime = 0;

    // Initial data fill ins
    this->initGrid();
    
    // Main loop for filling new points while not completed
    while (!this->isComplete() && this->mRuntime < MAX_RUNTIME) {
        // Check columns
        for (int i = 0; i < this->mColCount; i++) {
            this->check(TYPE::COL, i, this->mGrid.col(i));
        }
        
        // Check rows
        for (int i = 0; i < this->mRowCount; i++) {
            this->check(TYPE::ROW, i, this->mGrid.row(i));
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
    auto head = line.head();
    int remove = line.size() - (head.size() - 1);
    for (int i = 0; i < head.size(); i++) {
        remove -= head[i];
    }
    return remove;
}

void Solver::initGrid() noexcept {
    // Col data
    {
        // Loop through cols
        for (int lineNum = 0; lineNum < this->mColCount; lineNum++) {
            auto curCol = this->mGrid.col(lineNum);

            // Check if column is empty
            if (curCol.head()[0] == 0) {
                this->fill(TYPE::COL, lineNum, 0, curCol.size(), STATE::INVALID);
                continue;
            }
            
            int remain = calcRemaining(this->mGrid.col(lineNum));
            
            // All squares should be filled
            if (remain == 0) {
                // Fill in data
                this->initCompleteLine(TYPE::COL, lineNum, curCol);
                
                // Completes dataset
                this->mSolvedLines++;
            }
            else {
                // Data doesn't fully fill section
                this->initIncompleteLine(TYPE::COL, lineNum, remain, curCol);
            }
        }
    }
    // Row data
    {
        // Loop through cols
        for (int lineNum = 0; lineNum < this->mRowCount; lineNum++) {
            auto curRow = this->mGrid.row(lineNum);

            // Check if column is empty
            if (curRow.head()[0] == 0) {
                this->fill(TYPE::ROW, lineNum, 0, curRow.size(), STATE::INVALID);
                continue;
            }

            int remain = calcRemaining(this->mGrid.row(lineNum));
            
            // All squares should be filled
            if (remain == 0) {
                // Fill in data
                this->initCompleteLine(TYPE::ROW, lineNum, curRow);
                
                // Completes dataset
                this->mSolvedLines++;
            }
            else {
                // Data doesn't fully fill section
                this->initIncompleteLine(TYPE::ROW, lineNum, remain, curRow);
            }
        }
    }
}

inline void Solver::initCompleteLine(TYPE type, int lineNum, const Line& line) noexcept {
    // Loop through all data points and add them to grid
    int startIndex = 0;
    for(int i = 0; i < line.head().size(); i++) {
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
    for(int i = 0; i < line.head().size(); i++) {
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

void Solver::check(TYPE type, int lineNum, const Line& line) noexcept {
    // See if complete
    if (this->isComplete(line)) {
        return;
    }

    // Check sections
    int sections = this->lineSections((type == TYPE::COL ? this->mGrid.col(lineNum): this->mGrid.row(lineNum)));
    if (sections == line.sections()) {
        // Split into smaller lines
        Array<Line> split = lineSplit(line);
        for (int i = 0; i < split.size(); i++) {
            this->lineSectionSolve(type, lineNum, split[i]);
        }
    }
}

void Solver::lineSectionSolve(TYPE type, int lineNum, const Line& line) noexcept {
    // Check if section complete
    int remain = calcRemaining(line);
    if (remain == 0) {
        this->completeLineSection(type, lineNum, line);
        this->mSolvedLines++;
    }
    else {
        this->incompleteLineSection(type, lineNum, remain, line);
    }
}

void Solver::completeLineSection(TYPE type, int lineNum, const Line& line) noexcept {
    // Sets the valid data
    this->fill(type, lineNum, line.start(), line.size(), STATE::VALID);
}

void Solver::incompleteLineSection(TYPE type, int lineNum, int remain, const Line& line) noexcept {
    // Stores index for filling data
    int startIndex = 0;
    for(int i = 0; i < line.head().size(); i++) {
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

// ----- Other -----

void Solver::fill(TYPE t, int line, int start, int spaces, STATE state) noexcept {
    if (t == TYPE::COL) {
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
    else if (t == TYPE::ROW) {
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

