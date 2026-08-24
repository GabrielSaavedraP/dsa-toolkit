#pragma once
#include <cstring>
#include <ostream>

// MyString: dynamic char array, replacement for std::string.
//
// HOW IT WORKS
// Same mechanism as MyVector, specialized to char: heap array `data_`
// of size (len_ + 1), where the extra slot always holds '\0' so the
// buffer can still be treated like a C-string when needed (c_str()).
// Every operation that produces a new string (concat, substr) builds a
// fresh buffer and returns a new MyString — Rule of Three applies here
// exactly like MyVector: never share the raw pointer between objects.
//
// Complexity: length O(1), operator[] O(1), concat/substr/equals O(n).
class MyString {
private:
    char* data_;
    int len_;

    static int cstrlen(const char* s) {
        int n = 0;
        while (s[n] != '\0') n++;
        return n;
    }

public:
    MyString(const char* s = "") {
        len_ = cstrlen(s);
        data_ = new char[len_ + 1];              // +1 for the terminator
        for (int i = 0; i <= len_; i++) data_[i] = s[i];
    }

    MyString(const MyString& other) : len_(other.len_) {
        data_ = new char[len_ + 1];
        for (int i = 0; i <= len_; i++) data_[i] = other.data_[i];
    }

    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;
        delete[] data_;
        len_ = other.len_;
        data_ = new char[len_ + 1];
        for (int i = 0; i <= len_; i++) data_[i] = other.data_[i];
        return *this;
    }

    ~MyString() { delete[] data_; }

    int length() const { return len_; }
    bool empty() const { return len_ == 0; }
    const char* c_str() const { return data_; }

    char& operator[](int i) { return data_[i]; }
    char operator[](int i) const { return data_[i]; }

    void push_back(char c) {
        char* newData = new char[len_ + 2];
        for (int i = 0; i < len_; i++) newData[i] = data_[i];
        newData[len_] = c;
        newData[len_ + 1] = '\0';
        delete[] data_;
        data_ = newData;
        len_++;
    }

    MyString concat(const MyString& other) const {
        char* buf = new char[len_ + other.len_ + 1];
        for (int i = 0; i < len_; i++) buf[i] = data_[i];
        for (int i = 0; i < other.len_; i++) buf[len_ + i] = other.data_[i];
        buf[len_ + other.len_] = '\0';
        MyString result(buf);
        delete[] buf;
        return result;
    }

    MyString operator+(const MyString& other) const { return concat(other); }

    bool equals(const MyString& other) const {
        if (len_ != other.len_) return false;
        for (int i = 0; i < len_; i++)
            if (data_[i] != other.data_[i]) return false;
        return true;
    }
    bool operator==(const MyString& other) const { return equals(other); }

    MyString substr(int start, int count) const {
        char* buf = new char[count + 1];
        for (int i = 0; i < count; i++) buf[i] = data_[start + i];
        buf[count] = '\0';
        MyString result(buf);
        delete[] buf;
        return result;
    }

    // Naive substring search, O(n*m). Returns index of first match or -1.
    int find(const MyString& needle) const {
        if (needle.len_ == 0) return 0;
        for (int i = 0; i + needle.len_ <= len_; i++) {
            bool match = true;
            for (int j = 0; j < needle.len_; j++) {
                if (data_[i + j] != needle.data_[j]) { match = false; break; }
            }
            if (match) return i;
        }
        return -1;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
        os << s.data_;
        return os;
    }
};
