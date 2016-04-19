#pragma once

#include <string>
#include <vector>
#include <memory>

namespace mocca {
namespace net {
using MessagePart = std::shared_ptr<std::vector<uint8_t>>;
using Message = std::vector<MessagePart>;

MessagePart createMessagePart(const std::string& str);
std::string readMessagePart(const std::vector<uint8_t>& part);

template <typename T> void appendToMessagePart(std::vector<uint8_t>& messagePart, T value) {
    auto oldSize = messagePart.size();
    messagePart.resize(messagePart.size() + sizeof(T));
    *reinterpret_cast<T*>(messagePart.data() + oldSize) = value;
}

template <typename T> const uint8_t* readFromMessagePart(const uint8_t* const data, T& value) {
    value = *reinterpret_cast<const T* const>(data);
    return data + sizeof(T);
}

}
}