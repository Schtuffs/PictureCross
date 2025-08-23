#pragma once

#include <iostream>

typedef struct TimeData {
    long begin, end;
    double total;

    friend std::ostream& operator<<(std::ostream& cout, const TimeData& data);
} TimeData;

namespace Timer {
    // Pushes a new timer onto the stack
    void begin();
    // Pops most recent time struct off the stack
    TimeData end();
}

