#pragma once

#include "Array.h"
#include "HeaderInfo.h"
#include "Grid.h"

// Output

void mainMenu();
void setConsoleColour(COLOUR col);
void print(const HeaderInfo& info, const Grid& grid);

// Input

int intInput(const std::string& text, int min, int max);
Array<int> multiIntInput(const std::string& text, int total, int min, int max);

void determineInput(HeaderInfo& info, int input);

