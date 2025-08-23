#include "Solver.h"

#include "Defines.h"
#include "Timer.h"

static bool sEndLoop = false;

// ----- Creation ----- Destruction -----

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
BOOL WINAPI EventHandler(DWORD dwType) {
    switch (dwType) {
        case CTRL_C_EVENT:
            std::cout << "Ending solve\n";
            sEndLoop = true;
            return TRUE;
        default:
            break;
    }
    return FALSE;
}
#endif

Solver::Solver(const HeaderInfo& info) : mInfo(info), mGrid(info.allCols(), info.allRows()), mSolvedLines(0), mColCount(info.col()), mRowCount(info.row()), mColSize(mInfo.row()), mRowSize(mInfo.col()), mSolvedCols(this->mColCount), mSolvedRows(this->mRowCount) {
    // Mount event handler
#ifdef _WIN32
    if (!SetConsoleCtrlHandler(EventHandler, true)) {
        std::cout << "Failed to set Ctrl+C event handler\n";
    }
#endif
}
Solver::~Solver() {
    // Unmount event handler
#ifdef _WIN32
    if (!SetConsoleCtrlHandler(EventHandler, false)) {
        std::cout << "Failed to remove Ctrl+C event handler\n";
    }
#endif
}

void Solver::reset() {
    // Reset solved cols and rows
    for (int i = 0; i < this->mSolvedCols.size(); i++) { this->mSolvedCols[i] = false; }
    for (int i = 0; i < this->mSolvedRows.size(); i++) { this->mSolvedRows[i] = false; }

    // New grid
    this->mGrid = Grid(this->mInfo.allCols(), this->mInfo.allRows());

    // Other data
    this->mSolvedLines = 0;
}



// ----- Read -----

double Solver::runtime() const noexcept {
    return this->mRuntime;
}



// ----- Read ----- Private -----

bool Solver::isComplete() {
    return (this->mSolvedLines == (this->mColSize + this->mRowSize));
}

bool Solver::isComplete(const LineData& data) noexcept {
    // Check if complete
    if (data.type == TYPE::COL) {
        if (this->mSolvedCols[data.lineNum]) {
            return true;
        }
    }
    if (data.type == TYPE::ROW) {
        if (this->mSolvedRows[data.lineNum]) {
            return true;
        }
    }
    
    // Get valid squares
    int valid = 0;
    for (int i = 0; i < data.line.size(); i++) {
        if (data.line[i] == STATE::VALID) {
            valid++;
        }
    }

    // Get head data
    int exp = 0;
    auto head = data.line.head();
    for (int i = 0; i < head.size(); i++) {
        exp += head[i];
    }

    // Fill in if required
    if (valid == exp) {
        // Mark completion
        this->markCompletion(data);

        return true;
    }
    return false;
    
}

int Solver::lineSections(const Line& line) const noexcept {
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
    int searchIndex = 0, startIndex = 0;
    bool inNewSection = false;
    for (int i = 0; i < lines.size(); i++) {
        // Find the size of the line
        int size = 0;
        while (searchIndex < line.size()) {
            if (line[searchIndex++] != STATE::INVALID) {
                if (!inNewSection) {
                    startIndex = searchIndex - 1;
                    inNewSection = true;
                }
                size++;
            }
            else {
                // Ensure line has a size, otherwise started on bad square
                if (size == 0) {
                    continue;
                }
                inNewSection = false;
                break;
            }
        }
        Array<int> head(1);
        head[0] = line.head()[i];
        lines[i] = Line(size, head);
        lines[i].start(startIndex);
    }

    return lines;
}



// ----- Update -----

const Grid& Solver::solve() {
    std::cout << "\nBeginning solve...\n";

    // Reset data in case not new solver
    this->reset();
    
    // Begin solve time
    clock_t begin = clock();
    this->mRuntime = 0;
    
    // Initial data fill ins
    this->initGrid();
    
    // Main loop for filling new points while not completed
    int loop = 1;
    char buf[10];
    while (!this->isComplete() && this->mRuntime < MAX_RUNTIME * 15 && !sEndLoop) {
        // Check columns
        for (int i = 0; i < this->mColCount; i++) {
            this->check(LineData{TYPE::COL, i, this->mGrid.col(i)});
        }
        
        // Check rows
        for (int i = 0; i < this->mRowCount; i++) {
            this->check(LineData{TYPE::ROW, i, this->mGrid.row(i)});
        }
        
        // Runtime check
        clock_t current = clock();
        this->mRuntime = (double)(current - begin) / (double)CLOCKS_PER_SEC;
        loop++;
    }

    sEndLoop = false;
    return this->mGrid;
}



// ----- Update ----- Private -----

void Solver::markCompletion(const LineData& data) noexcept {
    // Mark completion
    (data.type == TYPE::COL ? this->mSolvedCols[data.lineNum] = true : this->mSolvedRows[data.lineNum] = true);
    this->mSolvedLines++;

    // Fill in complete line
    this->fill(data.type, data.lineNum, 0, data.line.size(), STATE::INVALID);
}

// ----- Initializing -----

// Calculates the remaining number of squares in the initial header data
int calcRemaining(const Line& line) noexcept {
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
                this->initCompleteLine({ TYPE::COL, lineNum, curCol });
                
                // Completes dataset
                this->mSolvedLines++;
            }
            else {
                // Data doesn't fully fill section
                this->initIncompleteLine({TYPE::COL, lineNum, curCol }, remain);
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
                this->initCompleteLine({TYPE::ROW, lineNum, curRow});
                
                // Completes dataset
                this->mSolvedLines++;
            }
            else {
                // Data doesn't fully fill section
                this->initIncompleteLine({TYPE::ROW, lineNum, curRow}, remain);
            }
        }
    }
}

void Solver::initCompleteLine(const LineData& data) noexcept {
    // Loop through all data points and add them to grid
    int startIndex = 0;
    for(int i = 0; i < data.line.head().size(); i++) {
        // Sets the valid data
        this->fill(data.type, data.lineNum, startIndex, data.line.head()[i], STATE::VALID);

        // Moves the start index for the invalid square
        startIndex += data.line.head()[i];

        // Adds the invalid square
        this->fill(data.type, data.lineNum, startIndex, 1, STATE::INVALID);

        // Final update for after invalid square
        startIndex++;
    }
}

void Solver::initIncompleteLine(const LineData& data, int remain) noexcept {
    // Stores index for filling data
    int startIndex = 0;
    for(int i = 0; i < data.line.head().size(); i++) {
        // Number of datapoints to fill
        int valueToFill = data.line.head()[i];

        // Removes extra points for proper resizing
        valueToFill -= remain;

        // Fill in section data
        this->fill(data.type, data.lineNum, startIndex + remain, valueToFill, STATE::VALID);

        // Start index increases by the data value and extra space
        startIndex += data.line.head()[i] + 1;
    }
}

// ----- Looping -----

void Solver::check(const LineData& data) noexcept {
    // See if complete
    if (this->isComplete(data)) {
        return;
    }

    // Remove areas that are too small to hold data
    this->removeSmallSections(data);

    // Check sections are equal to required sections
    int sections = this->lineSections(data.line);
    if (sections == data.line.sections()) {
        // Split into smaller lines
        Array<Line> split = lineSplit(data.line);
        for (int i = 0; i < split.size(); i++) {
            this->lineSectionSolve(LineData{ .type = data.type, .lineNum = data.lineNum, .line = split[i] });
        }
    }

    // Fill in guarenteed edge data or complete edges
    this->lineEdgeCheck(data);
}

void Solver::removeSmallSections(const LineData& data) noexcept {
    // Find min
    int min = data.line.head()[0];
    for (int i = 1; i < data.line.head().size(); i++) {
        if (min > data.line.head()[i]) {
            min = data.line.head()[i];
        }
    }

    // Check all sections are greater than min
    int secStart = 0, totSq = 0;
    bool inSec = false, inValidSec = false;
    for (int i = 0; i < data.line.size(); i++) {
        switch (data.line[i]) {
            case STATE::VALID:
                inValidSec = true;
            case STATE::NONE:
                // Put into section
                if (!inSec) {
                    secStart = i;
                }
                inSec = true;
                totSq++;
                break;

            case STATE::INVALID:
                // If not in section, continue searching for section
                if (!inSec) {
                    continue;
                }

                // Begin calculations
                inSec = false;

                // If in valid, do nothing since it will have data
                if (inValidSec) {
                    inValidSec = false;
                    continue;
                }

                // Check squares
                if (totSq < min) {
                    // Too small
                    this->fill(data.type, data.lineNum, secStart, totSq, STATE::INVALID);
                }
                
                inValidSec = false;
                totSq = 0;
                break;
            }
    }

    // Final check for edge of line
    if (inSec && !inValidSec) {
        // Check squares
        if (totSq < min) {
            // Too small
            this->fill(data.type, data.lineNum, secStart, totSq, STATE::INVALID);
        }
    }
}

void Solver::lineSectionSolve(const LineData& data) noexcept {
    // Check if section complete
    int remain = calcRemaining(data.line);
    if (remain == 0) {
        this->completeLineSection(data);
    }
    else {
        this->incompleteLineSection(data, remain);
    }
}

void Solver::completeLineSection(const LineData& data) noexcept {
    // Sets the valid data
    this->fill(data.type, data.lineNum, data.line.start(), data.line.size(), STATE::VALID);
}

void Solver::incompleteLineSection(const LineData& data, int remain) noexcept {
    // Fill in section data             (start of opening + remain)  (size of opening - remain)
    this->fill(data.type, data.lineNum, data.line.start() + remain, data.line.head()[0] - remain, STATE::VALID);
}

void Solver::lineEdgeCheck(const LineData& data) noexcept {
    // Check for left edge
    for (int i = 0; i < data.line.head()[0]; i++) {
        // Find a valid square
        // if ()
    }
}

// ----- Other -----

void Solver::fill(TYPE t, int line, int start, int spaces, STATE state) noexcept {
    int lineMax = (t == TYPE::COL ? this->mInfo.row() : this->mInfo.col());

    // Ensures data begins within bounds of the array
    if (start >= lineMax) {
        return;
    }

    // Loop through spaces
    for(int i = 0; i < spaces; i++) {
        // Setup selected column and row data
        int col, row;
        if (t == TYPE::COL) {
            col = line;
            row = i + start;
        }
        else {
            col = i + start;
            row = line;
        }

        // Ensures that data can't out of index during looping
        if (i + start >= lineMax) {
            return;
        }

        // Checks index is valid
        if (i + start < 0) {
            continue;
        }
        
        // Check that spot isn't already filled in
        if (this->mGrid.get(col, row) != STATE::NONE) {
            continue;
        }

        // Sets data index to be proper state
        this->mGrid.set(col, row, state);
    }
}

