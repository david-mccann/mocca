#pragma once

#include <memory>
#include <vector>

namespace mocca {
namespace net {

class Message {
public:
    virtual const uint8_t* data() const = 0;
    virtual uint32_t size() const = 0;
    virtual bool isEmpty() const = 0;

    void append(std::unique_ptr<Message> next);

private:
    std::unique_ptr<Message> next_;
};

class MessageShared : public Message {
public:
    MessageShared() = default;
    MessageShared(std::shared_ptr<const std::vector<uint8_t>> data);

    const uint8_t* data() const override;
    uint32_t size() const override;
    bool isEmpty() const override;

    std::shared_ptr<const std::vector<uint8_t>> sharedData() const;

private:
    std::shared_ptr<const std::vector<uint8_t>> data_;
};

class MessageUnique : public Message {
public:
    MessageUnique() = default;
    MessageUnique(std::vector<uint8_t> data);

    const uint8_t* data() const override;
    uint32_t size() const override;
    bool isEmpty() const override;

private:
    std::vector<uint8_t> data_;
};
}
}