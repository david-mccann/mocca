#pragma once

#include <memory>
#include <vector>

namespace mocca {
namespace net {
class Message {
public:
    Message() = default;
    Message(std::shared_ptr<const std::vector<uint8_t>> data);

    Message& setNext(std::shared_ptr<const std::vector<uint8_t>> data);
    const Message* next() const;

    const uint8_t* data() const;
    std::shared_ptr<const std::vector<uint8_t>> sharedData() const;
    uint32_t size() const;
    bool isEmpty() const;

private:
    std::shared_ptr<const std::vector<uint8_t>> data_;
    std::unique_ptr<Message> next_;
};
}
}