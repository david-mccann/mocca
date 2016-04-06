#include "mocca/net/Message.h"

#include "mocca/base/Memory.h"

using namespace mocca::net;

void Message::append(std::unique_ptr<Message> next) {
    next_ = std::move(next);
}

MessageShared::MessageShared(std::shared_ptr<const std::vector<uint8_t>> data)
    : data_(data) {}

const uint8_t* MessageShared::data() const {
    return data_->data();
}

uint32_t MessageShared::size() const {
    return data_->size();
}

bool MessageShared::isEmpty() const {
    return data_ == nullptr;
}

std::shared_ptr<const std::vector<uint8_t>> MessageShared::sharedData() const {
    return data_;
}


MessageUnique::MessageUnique(std::vector<uint8_t> data)
    : data_(std::move(data)) {}

const uint8_t* MessageUnique::data() const {
    return data_.data();
}

uint32_t MessageUnique::size() const {
    return data_.size();
}

bool MessageUnique::isEmpty() const {
    return data_.empty();
}