#include "mocca/base/ByteArray.h"

#include <algorithm>
#include <limits>

static_assert(std::numeric_limits<float>::is_iec559 == true, "Unsupported floating point type");
static_assert(std::numeric_limits<double>::is_iec559 == true, "Unsupported floating point type");

namespace mocca {

ByteArray::ByteArray(uint32_t capacity)
    : data_(new unsigned char[capacity])
    , capacity_(capacity)
    , size_(0)
    , readPos_(0) {}

ByteArray::ByteArray(ByteArray&& other)
    : data_(std::move(other.data_))
    , capacity_(other.capacity_)
    , size_(other.size_)
    , readPos_(other.readPos_) {
    other.capacity_ = 0;
    other.size_ = 0;
    other.readPos_ = 0;
}

void swap(ByteArray& lhs, ByteArray& rhs) {
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.capacity_, rhs.capacity_);
    swap(lhs.size_, rhs.size_);
    swap(lhs.readPos_, rhs.readPos_);
}

ByteArray mocca::ByteArray::clone() const {
    return ByteArray::createFromRaw(data(), size());
}

unsigned char* ByteArray::data() {
    return data_.get();
}

const unsigned char* ByteArray::data() const {
    return data_.get();
}

uint32_t ByteArray::size() const {
    return size_;
}

void ByteArray::setSize(uint32_t size) {
    if (size > capacity_) {
        throw Error("Size exceeds capacity of the byte array", __FILE__, __LINE__);
    }
    size_ = size;
}

bool ByteArray::isEmpty() const {
    return size_ == 0;
}

uint32_t ByteArray::capacity() const {
    return capacity_;
}

void ByteArray::resize(uint32_t newCapacity) {
    auto newData = std::unique_ptr<unsigned char[]>(new unsigned char[newCapacity]);
    memcpy(newData.get(), data_.get(), size_);
    data_ = std::move(newData);
    capacity_ = newCapacity;
}

void ByteArray::append(const void* data, uint32_t size) {
    if (capacity_ < size_ + size) {
        resize(size_ + std::max(2 * size, 256u));
    }
    memcpy(data_.get() + size_, data, size);
    size_ += size;
}

void mocca::ByteArray::append(const ByteArray& byteArray) {
    append(byteArray.data(), byteArray.size());
}

ByteArray ByteArray::createFromRaw(const void* raw, uint32_t size) {
    ByteArray byteArray(size);
    memcpy(byteArray.data_.get(), raw, size);
    byteArray.size_ = size;
    return std::move(byteArray);
}

ByteArray& ByteArray::operator<<(unsigned char val) {
    append(&val, sizeof(unsigned char));
    return *this;
}

ByteArray& ByteArray::operator>>(unsigned char& val) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (readPos_ + sizeof(unsigned char) > size_) {
        throw Error("Reading beyond end of packet", __FILE__, __LINE__);
    }
#endif
    memcpy(&val, data_.get() + readPos_, sizeof(unsigned char));
    readPos_ += sizeof(unsigned char);
    return *this;
}

ByteArray& ByteArray::operator<<(int16_t val) {
    append(&val, sizeof(int16_t));
    return *this;
}

ByteArray& ByteArray::operator>>(int16_t& val) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (readPos_ + sizeof(int16_t) > size_) {
        throw Error("Reading beyond end of packet", __FILE__, __LINE__);
    }
#endif
    memcpy(&val, data_.get() + readPos_, sizeof(int16_t));
    readPos_ += sizeof(int16_t);
    return *this;
}

ByteArray& ByteArray::operator<<(uint16_t val) {
    return (*this << (int16_t)val);
}

ByteArray& ByteArray::operator>>(uint16_t& val) {
    return (*this >> (int16_t&)val);
}

ByteArray& ByteArray::operator<<(int32_t val) {
    append(&val, sizeof(int32_t));
    return *this;
}

ByteArray& ByteArray::operator>>(int32_t& val) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (readPos_ + sizeof(int32_t) > size_) {
        throw Error("Reading beyond end of packet", __FILE__, __LINE__);
    }
#endif
    memcpy(&val, data_.get() + readPos_, sizeof(int32_t));
    readPos_ += sizeof(int32_t);
    return *this;
}

ByteArray& ByteArray::operator<<(uint32_t val) {
    return (*this << (int32_t)val);
}

ByteArray& ByteArray::operator>>(uint32_t& val) {
    return (*this >> (int32_t&)val);
}

ByteArray& ByteArray::operator<<(int64_t val) {
    append(&val, sizeof(int64_t));
    return *this;
}

ByteArray& ByteArray::operator>>(int64_t& val) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (readPos_ + sizeof(int64_t) > size_) {
        throw Error("Reading beyond end of packet", __FILE__, __LINE__);
    }
#endif
    memcpy(&val, data_.get() + readPos_, sizeof(int64_t));
    readPos_ += sizeof(int64_t);
    return *this;
}

ByteArray& ByteArray::operator<<(uint64_t val) {
    return (*this << (int64_t)val);
}

ByteArray& ByteArray::operator>>(uint64_t& val) {
    return (*this >> (int64_t&)val);
}

ByteArray& ByteArray::operator<<(float val) {
    append(&val, sizeof(float));
    return *this;
}

ByteArray& ByteArray::operator>>(float& val) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (readPos_ + sizeof(float) > size_) {
        throw Error("Reading beyond end of packet", __FILE__, __LINE__);
    }
#endif
    memcpy(&val, data_.get() + readPos_, sizeof(float));
    readPos_ += sizeof(float);
    return *this;
}

ByteArray& ByteArray::operator<<(double val) {
    append(&val, sizeof(double));
    return *this;
}

ByteArray& ByteArray::operator>>(double& val) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (readPos_ + sizeof(double) > size_) {
        throw Error("Reading beyond end of packet", __FILE__, __LINE__);
    }
#endif
    memcpy(&val, data_.get() + readPos_, sizeof(double));
    readPos_ += sizeof(double);
    return *this;
}

ByteArray& ByteArray::operator<<(const std::string& val) {
    append(val.c_str(), (uint32_t)val.size());
    return *this;
}

ByteArray& ByteArray::operator<<(const char* val) {
    return (*this << std::string(val));
}

unsigned char& mocca::ByteArray::operator[](uint32_t index) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (index >= size_) {
        throw Error("Index out of bounds", __FILE__, __LINE__);
    }
#endif
    return data_[index];
}

const unsigned char& mocca::ByteArray::operator[](uint32_t index) const {
#ifdef MOCCA_RUNTIME_CHECKS
    if (index >= size_) {
        throw Error("Index out of bounds", __FILE__, __LINE__);
    }
#endif
    return data_[index];
}

std::string ByteArray::read(uint32_t size) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (readPos_ + size > size_) {
        throw Error("Reading beyond end of packet", __FILE__, __LINE__);
    }
#endif
    std::string result((char*)(data_.get() + readPos_), size);
    readPos_ += size;
    return result;
}

void ByteArray::resetReadPos() {
    readPos_ = 0;
}

std::string readAt(const ByteArray& byteArray, uint32_t index, uint32_t size) {
#ifdef MOCCA_RUNTIME_CHECKS
    if (index + size > byteArray.size()) {
        throw Error("Index out of bounds", __FILE__, __LINE__);
    }
#endif
    return std::string((char*)(byteArray.data() + index), size);
}
}