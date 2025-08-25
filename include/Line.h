#pragma once

#include "Array.h"
#include "Defines.h"

class Line {
private:
    char mSize, mStartIndex;
    Array<int> mHead;
    Array<STATE> mCells;

public:
    Line();
    Line(int size);
    Line(int size, const Array<int>& head);
    ~Line();

    // ----- Read -----

    // Get the header content for the line
    const Array<int>& head() const noexcept;
    // Gets the length of the line
    int size() const noexcept;
    // Gets the number of sections this line expects to have
    int sections() const noexcept;
    // Retrieves specified cell data, bounds checks
    STATE get(int i) const noexcept;
    // Gets the start index, only needed for sublines
    int start() const noexcept;

    // ----- Update -----
    
    // Tries to set specified cell to given state
    bool set(int index, STATE state) noexcept;
    // Set the header content for the line
    void head(const Array<int>& head) noexcept;
    // Changes the start index, only needed for sublines
    void start(int index) noexcept;

    // ----- Others -----
    
    STATE& operator[](int index) const noexcept;
    friend std::ostream& operator<<(std::ostream& cout, const Line& line);
};

