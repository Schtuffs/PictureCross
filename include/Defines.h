#pragma once
#include <thread>

#ifdef _WIN32
    #define CON_CLEAR system("cls")
    #elif __linux__
    #define SLEEP(time) sleep(time);
    #define CON_CLEAR system("clear")
#endif

// Display colours
enum class COLOUR { RESET, BLACK = 30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

// Menu codes
enum class CODES { EXIT, COLUMN, ROW, SOLVE, TOTAL, ERROR };

// Others

enum class TYPE { COL, ROW };

constexpr double MAX_RUNTIME   = 1.0;

