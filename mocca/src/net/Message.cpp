#include "mocca/net/Message.h"

#include "mocca/base/Memory.h"

using namespace mocca::net;

Message::Message(std::shared_ptr<const std::vector<uint8_t>> data)
    : data_(data), next_(nullptr) {}

Message& Message::setNext(std::shared_ptr<const std::vector<uint8_t>> data) {
    next_ = mocca::make_unique<Message>(data);
    return *next_;
}

const Message* Message::next() const {
    return next_.get();
}

const uint8_t* Message::data() const {
    return data_->data();
}

uint32_t Message::size() const {
    return data_->size();
}

bool Message::isEmpty() const {
    return data_ == nullptr;
}

std::shared_ptr<const std::vector<uint8_t>> Message::sharedData() const {
    return data_;
}