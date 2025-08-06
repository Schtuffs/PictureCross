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
    const Array<int>& head() const noexcept;
    // Gets the length of the line
    int size() const noexcept;
    // Gets the number of sections this line expects to have
    int sections() const noexcept;
    // Retrieves specified cell data
    STATE get(int i) const noexcept;
    
    // Tries to set specified cell to given state
    bool set(int index, STATE state) noexcept;
    // Set the header content for the line
    void head(const Array<int>& head) noexcept;
    
    STATE operator[](int index);
};

