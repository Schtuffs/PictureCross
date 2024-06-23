#include <iostream>
#include <limits>
#include "../include/PictureCrossGrid.h"

#define EXITCODE    0
#define ERRORCODE   0x7FFFFFFF

void mainMenu();
int getIntInput(int min, int max);
void determineUserSelection(PictureCrossGrid& grid, int userInput);
void setRow(PictureCrossGrid& grid);
void setColumn(PictureCrossGrid& grid);
void getRow(PictureCrossGrid& grid);
void getColumn(PictureCrossGrid& grid);
void clearGrid(PictureCrossGrid& grid);
void quickSolve(PictureCrossGrid& grid);
void bruteSolve(PictureCrossGrid& grid);

static bool endProgram = false;

int main(void) {
    // Gets width and heigh of grid from user
    std::cout << "Enter the number of columns in the grid: " << std::endl;
    int cols = getIntInput(1, 99);
    std::cout << "Enter the number of rows in the grid: " << std::endl;
    int rows = getIntInput(1, 99);
    
    PictureCrossGrid grid(cols, rows);
    do {
        mainMenu();
        int userInput = getIntInput(1, 6);
        determineUserSelection(grid, userInput);
    } while (!endProgram);

    std::cout << "Exiting..." << std::endl;
    
    return 0;
}

void mainMenu() {
    std::cout << "Main Menu"                                                    << std::endl;
    std::cout << "1) Set a row"                                                 << std::endl;
    std::cout << "2) Set a column"                                              << std::endl;
    std::cout << "3) Get a row"                                                 << std::endl;
    std::cout << "4) Get a column"                                              << std::endl;
    std::cout << "5) Quick Solve"                                               << std::endl;
    std::cout << "6) Brute Solve (can take up to " << MAX_BRUTE_RUNTIME << "s)" << std::endl;
    std::cout << EXITCODE << ") Exit"                                           << std::endl;
}

int getIntInput(int min, int max) {
    int input = 0;
    std::cout << "Enter a number between " << min << " and " << max << "..." << std::endl;
    while (!(std::cin >> input) || (min > input || input > max)) {
        if (input == EXITCODE) {
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid entry. Enter a number between " << min << " and " << max << "..." << std::endl;
    }
    
    return input;
}

void determineUserSelection(PictureCrossGrid& grid, int userInput) {
    switch(userInput) {
        case 1:
            setRow(grid);
            break;
        case 2:
            setColumn(grid);
            break;
        case 3:
            getRow(grid);
            break;
        case 4:
            getColumn(grid);
            break;
        case 5:
            quickSolve(grid);
            break;
        case 6:
            bruteSolve(grid);
            break;
        case EXITCODE:
            endProgram = true;
            break;
        default:
            std::cout << "Not a valid option..." << std::endl;
            break;
    }
}

void setRow(PictureCrossGrid& grid) {
    int userInput;

    // Gets user to choose a row number to change
    do {
        std::cout << "Choose a row number." << std::endl;
        userInput = getIntInput(1, grid.getRowCount());

        if (userInput == EXITCODE)
            return;
        
    } while (0 > userInput || userInput > grid.getRowCount());
    // Saves users selected row
    int selectedRow = userInput;

    // Gets user to tell program how many values they will enter for the row
    do {
        std::cout << "Enter the number of values in the row." << std::endl;
        userInput = getIntInput(1, 5);

        if (userInput == EXITCODE)
            return;
        
    } while (0 > userInput || userInput > 5);

    int totalValues = userInput;
    int* data = new int[totalValues];

    // User enters the numbers for the row
    for(int i = 0; i < totalValues; i++) {
        std::cout << "Enter value #" << (i + 1) << " (Left to right, Up to down)." << std::endl;
        data[i] = getIntInput(1, grid.getColumnCount());

        if (userInput == EXITCODE) {
            delete [] data;
            return;
        }
    }

    // Checks that valid data was entered before setting
    int totalSpaces = 0;
    for(int i = 0; i < totalValues; i++) {
        totalSpaces += data[i];
        // Accounts for spaces
        if (i > 0)
            totalSpaces++;
    }
    if (totalSpaces > grid.getColumnCount()) {
        std::cout << "Numbers are too large. Not setting row..." << std::endl;
        delete [] data;
        return;
    }

    // Sets row after checking data
    grid.setRow(selectedRow, data, totalValues);
    std::cout << "Successfully set row!" << std::endl;
}

void setColumn(PictureCrossGrid& grid) {
    int userInput;

    // Gets user to choose a column number to change
    do {
        std::cout << "Choose a column number." << std::endl;
        userInput = getIntInput(1, grid.getColumnCount());

        if (userInput == EXITCODE)
            return;
        
    } while (0 > userInput || userInput > grid.getColumnCount());
    // Saves users selected column
    int selectedColumn = userInput;

    // Gets user to tell program how many values they will enter for the column
    do {
        std::cout << "Enter the number of values in the column." << std::endl;
        userInput = getIntInput(1, 5);

        if (userInput == EXITCODE)
            return;
        
    } while (0 > userInput || userInput > 5);

    int totalValues = userInput;
    int* data = new int[totalValues];

    // User enters the numbers for the row
    for(int i = 0; i < totalValues; i++) {
        std::cout << "Enter value #" << (i + 1) << " (Left to right, Up to down)." << std::endl;
        data[i] = getIntInput(1, grid.getRowCount());

        if (userInput == EXITCODE) {
            delete [] data;
            return;
        }
    }

    // Checks that valid data was entered before setting
    int totalSpaces = 0;
    for(int i = 0; i < totalValues; i++) {
        totalSpaces += data[i];
        // Accounts for spaces
        if (i > 0)
            totalSpaces++;
    }
    if (totalSpaces > grid.getRowCount()) {
        std::cout << "Numbers are too large. Not setting column..." << std::endl;
        delete [] data;
        return;
    }

    // Sets column after checking data
    grid.setColumn(selectedColumn, data, totalValues);
    std::cout << "Successfully set column!" << std::endl;
}

void getRow(PictureCrossGrid& grid) {
    std::cout << "Enter a row number: " << std::endl;
    int input = getIntInput(1, grid.getRowCount());
    
    int* data = grid.getRow(input);
    if (data == NULL) {
        return;
    }

    for(int i = 1; i <= data[0]; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

void getColumn(PictureCrossGrid& grid) {
    std::cout << "Enter a column number: " << std::endl;
    int input = getIntInput(1, grid.getColumnCount());

    int* data = grid.getColumn(input);
    if (data == NULL) {
        return;
    }

    for(int i = 1; i <= data[0]; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

void quickSolve(PictureCrossGrid& grid) {
    grid.quickSolve();
}

void bruteSolve(PictureCrossGrid& grid) {
    grid.bruteSolve();
}

