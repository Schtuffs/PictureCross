#include "Timer.h"

// Stack size, storable in char
constexpr unsigned char MAX_STACK = 255;
// Pointer for pushing and popping stack
static unsigned char stackPointer = 0;
// This holds each time data
static TimeData timeStack[MAX_STACK];

void Timer::begin() {
    // Check stack poiner in range
    if (stackPointer >= MAX_STACK) {
        throw std::string("Too many times pushed on stack");
    }

    // Push onto the stack
    TimeData data;
    data.begin = clock();
    data.end = data.begin;
    data.total = 0;
    timeStack[stackPointer++] = data;
}

TimeData Timer::end() {
    // Check stack pointer
    if (stackPointer == 0) {
        throw std::string("Too many times popped from stack"); 
    }

    // Pop off the stack
    TimeData data = timeStack[--stackPointer];
    data.end = clock();
    data.total = (data.end - data.begin) / CLOCKS_PER_SEC;
    return data;
}

std::ostream& operator<<(std::ostream& cout, const TimeData& data) {
    cout << "Begin: " << ((double)data.begin / CLOCKS_PER_SEC) << "s, End: " << ((double)data.end / CLOCKS_PER_SEC) << "s, Total: " << data.total << "s";
    return cout;
}

