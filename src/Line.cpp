#include "Line.h"

// ----- Creation ----- Destruction -----
Line::Line() : mSize(1), mStartIndex(0), mHead(1), mCells(mSize) {}
Line::Line(int size) : mSize(size), mStartIndex(0), mHead(1) {
    if (this->mSize < 1) {
        this->mSize = 1;
    }
    this->mCells = Array<STATE>(mSize);
}
Line::Line(int size, const Array<int>& head) : mSize(size), mStartIndex(0), mHead(head) {
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
    return this->mHead.size();
}

STATE Line::get(int i) const noexcept {
    // Check index
    if (0 > i || i > this->mSize) {
        return STATE::UNKNOWN;
    }
    return this->mCells[i];
}

int Line::start() const noexcept {
    return this->mStartIndex;
}

STATE Line::operator[](int index) const noexcept {
    return this->mCells[index];
}

std::ostream& operator<<(std::ostream& cout, const Line& line) {
    cout << "{ ";
    for (int i = 0; i < line.size(); i++) {
        switch (line[i]) {
        case STATE::VALID:
            cout << "V";
            break;
        case STATE::INVALID:
            cout << "I";
            break;
        case STATE::NONE:
            cout << "N";
            break;
        default:
            cout << "?";
            break;
        }

        if (i != line.size() - 1) {
            cout << ", ";
        }
    }
    cout << " }";
    return cout;
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

void Line::start(int index) noexcept {
    this->mStartIndex = index;
}

