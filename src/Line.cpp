#include "Line.h"

// ----- Creation ----- Destruction -----
Line::Line() : mSize(1), mSections(1), mHead(1), mCells(mSize) {}
Line::Line(int size) : mSize(size), mSections(1), mHead(1) {
    if (this->mSize < 1) {
        this->mSize = 1;
    }
    this->mCells = Array<STATE>(mSize);
}
Line::Line(int size, const Array<int>& head) : mSize(size), mSections(head.size()), mHead(head) {
    if (this->mSize < 1) {
        this->mSize = 1;
    }
    this->mCells = Array<STATE>(mSize);
}

Line::~Line() {}



// ----- Read -----

int Line::size() const noexcept {
    return this->mSize;
}

const Array<int>& Line::head() const noexcept {
    return this->mHead;
}

int Line::sections() const noexcept {
    return this->mSections;
}

STATE Line::get(int i) const noexcept {
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

bool Line::set(int index, STATE state) noexcept {
    // Check index
    if (0 > index || index >= this->mSize) {
        return false;
    }

    // Set state
    this->mCells[index] = state;
    return true;
}

void Line::head(const Array<int>& head) noexcept {
    this->mHead = head;
}

