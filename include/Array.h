#pragma once

#include <iostream>

template <typename T>
class Array {
private:
    T* mData;
    int mSize;

public:
    // Always creates at least one space
    Array(int n = 1) : mSize(n) {
        if (this->mSize <= 0) {
            this->mSize = 1;
        }

        this->mData = new T[this->mSize]();
    }
    
    Array(const Array& rhs) : mSize(rhs.mSize) {
        this->mData = new T[this->mSize]();
        for (int i = 0; i < this->mSize; i++) {
            this->mData[i] = rhs.mData[i];
        }
    }

    ~Array() {
        delete [] this->mData;
    }

    inline int size() const noexcept {
        return this->mSize;
    }

    inline T& operator[](int index) const noexcept {
        return this->mData[index];
    }

    inline Array& operator=(const Array& rhs) noexcept {
        delete [] this->mData;
        this->mSize = rhs.mSize;
        this->mData = new T[this->mSize]();
        for (int i = 0; i < this->mSize; i++) {
            this->mData[i] = rhs.mData[i];
        }
        return *this;
    }
};

