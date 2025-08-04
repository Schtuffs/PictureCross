#include "Line.h"

// ----- Creation ----- Destruction -----
Line::Line() : mSize(0), mSections(0), mHead(0), mCells(mSize) {}
Line::Line(int size) : mSize(size), mSections(0), mHead(0), mCells(mSize) {
    if (this->mSize < 0) {
        this->mSize = 0;
    }
}
Line::Line(int size, const Array<int>& head) : mSize(size), mSections(head.size()), mHead(head), mCells(mSize) {
    if (this->mSize < 0) {
        this->mSize = 0;
    }
}

Line::~Line() {}



// ----- Read -----

inline int Line::size() const noexcept {
    return this->mSize;
}

inline const Array<int>& Line::head() const noexcept {
    return this->mHead;
}

inline int Line::sections() const noexcept {
    return this->mSections;
}

inline STATE Line::get(int i) const noexcept {
    // Check index
    if (0 > i || i > this->mSize) {
        return STATE::UNKNOWN;
    }
    return this->mCells[i];
}

STATE Line::operator[](int index) {
    return this->mCells[index];
}



// ----- Update -----

inline bool Line::set(int index, STATE state) noexcept {
    // Check index
    if (0 > index || index >= this->mSize) {
        return false;
    }

    // Set state
    this->mCells[index] = state;
    return true;
}

inline void Line::head(const Array<int>& head) noexcept {
    this->mHead = head;
}

