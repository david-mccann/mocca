#include "mocca/net/Message.h"

using namespace mocca::net;

Message::Message(std::shared_ptr<const std::vector<uint8_t>> data)
    : data_(data), next_(nullptr) {}

void Message::append(std::unique_ptr<Message> message) {
    next_ = std::move(message);
}

std::shared_ptr<const std::vector<uint8_t>> Message::data() const {
    return data_;
}

const Message* Message::next() const {
    return next_.get();
}

bool Message::isEmpty() const {
    return data_ == nullptr;
}