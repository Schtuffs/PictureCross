#include <iostream>
#include <print>

#include "Defines.h"
#include "main.h"
#include "Solver.h"

int main(int argc, char** argv) {
    // Clear console
    CON_CLEAR;

    // User input for grid
    int totalCols, totalRows;

    // Get cols from user
    totalCols = intInput("Enter total number of columns: ", 1, 99);
    if (totalCols ==(int)CODES::EXIT) {
        return 0;
    }
    
    // Get rows from user
    totalRows = intInput("Enter total number of rows:    ", 1, 99);
    if (totalRows ==(int)CODES::EXIT) {
        return 0;
    }
    
    // Create header info
    HeaderInfo info(totalCols, totalRows);

    // Add data
    int input;
    do {
        // Print menu
        mainMenu();

        // Get input
        input = intInput("Please enter one of the options: ", 0,(int)CODES::TOTAL - 1);

        // Determine input
        determineInput(info, input);
    } while (input !=(int)CODES::EXIT);

    
    return 0;
}

void mainMenu() {
    std::cout << "Main menu\n";
    std::cout <<(int)CODES::COLUMN  << ". Set column\n";
    std::cout <<(int)CODES::ROW     << ". Set row\n";
    std::cout <<(int)CODES::SOLVE   << ". Solve\n";
    std::cout <<(int)CODES::EXIT   << ". Exit\n";
}

void setConsoleColour(COLOUR col) {
    std::cout << "\033[" << (int)col << "m";
}

void print(const HeaderInfo& info, const Grid& grid) {
    std::print("\n");

    // First, find largest column numbers
    int colSize = info.col(0).size();
    for (int col = 1; col < info.col(); col++)
        if (colSize < info.col(col).size())
            colSize = info.col(col).size();

    // Second, find largest row numbers
    int rowSize = info.row(0).size();
    for (int row = 1; row < info.row(); row++)
        if (rowSize < info.row(row).size())
            rowSize = info.row(row).size();

    // Next, print out spaced col info
    // This has an initial spacing based on row info
    for (int printRow = 0; printRow < colSize; printRow++) {
        // Initial spacing
        for (int i = 0; i < rowSize; i++)
            printf("   ");
        printf(" ");
        
        // Loop through all columns to see which should print this row
        for (int col = 0; col < info.col(); col++) {
            // Determine if/which info to print from column
            auto colInfo = info.col(col);
            int index = colInfo.size() - (colSize - printRow);

            // Has proper indexing for printing
            if (index >= 0)
                printf(" %2d", colInfo[index]);
            // Else add spaces for alignment
            else
                printf("   ");
        }
        printf("\n");
    }
    
    // Initial spacing
    for (int i = 0; i < rowSize; i++)
        printf("   ");
    // Print separator
    printf("X");
    for (int i = 0; i < info.col(); i++)
        printf("---");
    printf("\n");

    // Finally, print out rows
    for (int i = 0; i < info.row(); i++) {
        auto headerRow = info.row(i);
        // Initial spacing if not largest
        for (int j = rowSize - headerRow.size(); j > 0; j--)
            printf("   ");

        // Print the row information
        for (int j = 0; j < headerRow.size(); j++)
            printf("%2d ", headerRow[j]);
        
        // Print the grid row
        printf("|");
        auto gridRow = grid.row(i);
        for (int gI = 0; gI < gridRow.size(); gI++) {
            // Works in powershell and bash for ANSI colour codes
            switch (gridRow[gI]) {
                case STATE::VALID:
                    setConsoleColour(COLOUR::GREEN);
                    std::print(" ☐ ");
                    break;
                case STATE::INVALID:
                    setConsoleColour(COLOUR::RED);
                    std::print(" ☒ ");
                    break;
                case STATE::NONE:
                    setConsoleColour(COLOUR::CYAN);
                    std::print(" ? ");
                    break;
                default:
                    setConsoleColour(COLOUR::MAGENTA);
                    std::print("ERR");
                    break;
            }
        }
        setConsoleColour(COLOUR::RESET);
        printf("\n");
    }
}

int intInput(const std::string& text, int min, int max) {
    int input = 0;
    std::cout << text;

    // Loop until valid input is received
    while (!(std::cin >> input) || (min > input || input > max)) {
        // Valid input
        if (input == (int)CODES::EXIT && !std::cin.fail()) {
            break;
        }

        // Fix invalid input
        std::cin.clear();
        std::cin.ignore(0x7fffffff, '\n');
        std::cout << "Invalid entry. Enter a number between " << min << " and " << max << ": ";
    }
    
    return input;
}

Array<int> multiIntInput(const std::string& text, int total, int min, int max) {
    Array<int> vals(total);
    
    // Loop for input
    for (int i = 0; i < total; i++) {
        // TODO - User can enter 0
        vals[i] = intInput(text, min, max);
    }
    
    return vals;
}

void determineInput(HeaderInfo& info, int input) {
    switch ((CODES)input) {
        case CODES::COLUMN: {
            int col = intInput("Select a column: ", 1, info.col());
            int size = intInput("Enter the number of values: ", 1, info.row());
            auto vals = multiIntInput("Enter column info: ", size, 1, info.row());
            info.setCol(col, vals);
            break;
        }
        case CODES::ROW: {
            int row = intInput("Select a row: ", 1, info.row());
            int size = intInput("Enter the number of values: ", 1, info.col());
            auto vals = multiIntInput("Enter row info: ", size, 1, info.col());
            info.setRow(row, vals);
            break;
        }
        case CODES::SOLVE: {
            // Solve
            Solver solver(info);
            Grid grid = solver.solve();
            
            // Output
            print(info, grid);
            std::cout << "Total runtime: " << solver.runtime() << "s\n";
            break;
        }
        case CODES::EXIT: {
            std::cout << "Exiting\n";
            exit(EXIT_SUCCESS);
            break;
        }
        default:
            std::cout << "Unimplemented item selected\n";
            break;
    }
}

