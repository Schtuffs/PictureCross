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
    const Array<int>& head() const;
    // Gets the length of the line
    int size() const;
    // Gets the number of sections this line expects to have
    int sections() const;
    // Retrieves specified cell data, bounds checks
    STATE get(int i) const;
    // Gets the start index, only needed for sublines
    int start() const;

    // ----- Update -----
    
    // Tries to set specified cell to given state
    bool set(int index, STATE state);
    // Set the header content for the line
    void head(const Array<int>& head);
    // Changes the start index, only needed for sublines
    void start(int index);

    // ----- Others -----
    
    STATE& operator[](int index) const;
    friend std::ostream& operator<<(std::ostream& cout, const Line& line);
};

