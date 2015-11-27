#pragma once

#include "mocca/base/Error.h"

#include <memory>
#include <string>
#include <cstring>

namespace mocca {

class ByteArray {
public:
    ByteArray(uint32_t capacity = 256);
    virtual ~ByteArray() = default;

    ByteArray(const ByteArray& other);
    ByteArray(ByteArray&& other);
    ByteArray& operator=(ByteArray other);

    friend void swap(ByteArray& lhs, ByteArray& rhs);

    char* data();
    const char* data() const;

    uint32_t capacity() const;

    uint32_t size() const;
    void setSize(uint32_t size);
    bool isEmpty() const;

    static ByteArray createFromRaw(const void* raw, uint32_t size);
    void append(const void* data, uint32_t size);

    ByteArray& operator<<(int32_t val);
    ByteArray& operator>>(int32_t& val);

    ByteArray& operator<<(uint32_t val);
    ByteArray& operator>>(uint32_t& val);

    ByteArray& operator<<(int64_t val);
    ByteArray& operator>>(int64_t& val);

    ByteArray& operator<<(uint64_t val);
    ByteArray& operator>>(uint64_t& val);

    static const unsigned char falseConst = 0x01;
    static const unsigned char trueConst = 0x02;
    ByteArray& operator<<(bool val);
    ByteArray& operator>>(bool& val);

    ByteArray& operator<<(float val);
    ByteArray& operator>>(float& val);

    ByteArray& operator<<(double val);
    ByteArray& operator>>(double& val);

    ByteArray& operator<<(const char* val);
    ByteArray& operator<<(const std::string& val);
    ByteArray& operator>>(std::string& val);

    ByteArray& operator<<(const ByteArray& val);
    ByteArray& operator>>(ByteArray& val);

    template <typename T> T get() {
        T val;
        *this >> val;
        return val;
    }

    void resetReadPos();

private:
    void resize(uint32_t newCapacity);

    std::unique_ptr<char[]> data_;
    uint32_t capacity_;
    uint32_t size_;
    uint32_t readPos_;
};
}