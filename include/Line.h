#pragma once

#include "Array.h"

enum STATE { NONE, VALID, INVALID, UNKNOWN };

class Line {
private:
    char mSize, mSections;
    Array<int> mHead;
    Array<STATE> mCells;

public:
    Line();
    Line(int size);
    Line(int size, const Array<int>& head);
    ~Line();

    // Get the header content for the line
    inline const Array<int>& head() const noexcept;
    // Gets the length of the line
    inline int size() const noexcept;
    // Gets the number of sections this line expects to have
    inline int sections() const noexcept;
    // Retrieves specified cell data
    inline STATE get(int i) const noexcept;
    
    // Tries to set specified cell to given state
    inline bool set(int index, STATE state) noexcept;
    // Set the header content for the line
    inline void head(const Array<int>& head) noexcept;
    
    STATE operator[](int index);
};

