#pragma once

#include <iostream>

template <typename T>
class Array {
private:
    unsigned int mSize;
    T* mData;

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

    int size() const {
        return this->mSize;
    }

    T& operator[](int index) const {
        return this->mData[index];
    }

    Array& operator=(const Array& rhs) {
        delete [] this->mData;
        this->mSize = rhs.mSize;
        this->mData = new T[this->mSize]();
        for (int i = 0; i < this->mSize; i++) {
            this->mData[i] = rhs.mData[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& cout, const Array& arr) {
        cout << "{ ";
        for (int i = 0; i < arr.size(); i++) {
            cout << arr[i];
            if (i != arr.size() - 1) {
                cout << ", ";
            }
        }
        cout << " }";
        return cout;
    }
};

