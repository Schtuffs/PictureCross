#pragma once
#include <thread>

#ifdef _WIN32
    #define CON_CLEAR system("cls")
    #elif __linux__
    #define SLEEP(time) sleep(time);
    #define CON_CLEAR system("clear")
#endif

// Display colours
enum COLOUR { RESET, BLACK = 30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

enum CODES { EXIT, COLUMN, ROW, SOLVE, TOTAL, ERROR };

