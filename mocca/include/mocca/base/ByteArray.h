#pragma once

#include "mocca/base/Error.h"

#include <memory>
#include <string>
#include <cstring>
#include <tuple>

#define MOCCA_BYTEARRAY_CHECKS

namespace mocca {

class ByteArray {
public:
    ByteArray(uint32_t capacity = 256);
    ByteArray(const ByteArray& other) = delete;
    ByteArray(ByteArray&& other);

    friend void swap(ByteArray& lhs, ByteArray& rhs);

    ByteArray clone();

    unsigned char* data();
    const unsigned char* data() const;

    uint32_t capacity() const;

    uint32_t size() const;
    void setSize(uint32_t size);
    bool isEmpty() const;

    static ByteArray createFromRaw(const void* raw, uint32_t size);
    void append(const void* data, uint32_t size);
    void append(const ByteArray& byteArray);

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
    std::string read(uint32_t size);

    ByteArray& operator<<(const ByteArray& val);
    ByteArray& operator>>(ByteArray& val);

    unsigned char& operator[](uint32_t index);
    const unsigned char& operator[](uint32_t index) const;
    
    template <typename T> T read() {
        T val;
        *this >> val;
        return val;
    }

    void resetReadPos();

private:
    void resize(uint32_t newCapacity);

    std::unique_ptr<unsigned char[]> data_;
    uint32_t capacity_;
    uint32_t size_;
    uint32_t readPos_;
};

template <typename T> ByteArray makeFormattedByteArray(const T& value) {
    ByteArray result;
    result << value;
    return result;
}
template <> ByteArray makeFormattedByteArray<std::string>(const std::string& str);
template <> ByteArray makeFormattedByteArray<const char*>(const char* const& str);
template <typename T, typename... Args>
ByteArray makeFormattedByteArray(const T& value, const Args&... args) {
    ByteArray result = makeFormattedByteArray(value);
    result.append(makeFormattedByteArray(args...));
    return result;
}

template <typename T>
std::tuple<T> parseFormattedByteArray(ByteArray& byteArray) {
    T value;
    byteArray >> value;
    return std::tuple<T>(value);
}
template <> std::tuple<std::string> parseFormattedByteArray<std::string>(ByteArray& byteArray);
template <typename T, typename Arg, typename... Args>
std::tuple<T, Arg, Args...> parseFormattedByteArray(ByteArray& byteArray) {
    auto value = parseFormattedByteArray<T>(byteArray);
    return std::tuple_cat(value, parseFormattedByteArray<Arg, Args...>(byteArray));
}

}