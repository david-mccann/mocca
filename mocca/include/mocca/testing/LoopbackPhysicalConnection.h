#pragma once

#include <mutex>

namespace mocca {

template <typename T> class MessageQueue;
class ByteArray;

namespace net {

class LoopbackPhysicalConnection {
public:
    enum class Signal { Disconnect };

    using LoopbackMessageQueue = MessageQueue<unsigned char>;
    using LoopbackSignalQueue = MessageQueue<Signal>;

    LoopbackPhysicalConnection(std::shared_ptr<LoopbackMessageQueue> sendQueue, std::shared_ptr<LoopbackMessageQueue> receiveQueue,
                               std::shared_ptr<LoopbackSignalQueue> outSignalQueue, std::shared_ptr<LoopbackSignalQueue> inSignalQueue);
    ~LoopbackPhysicalConnection();

    std::string identifier() const;
    void lock();
    void unlock();
    void send(ByteArray message, std::chrono::milliseconds timeout = std::chrono::milliseconds(10)) const;
    ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(10)) const;

private:
    static std::string createIdentifier();

private:
    std::string identifier_;
    std::shared_ptr<LoopbackMessageQueue> sendQueue_;
    std::shared_ptr<LoopbackMessageQueue> receiveQueue_;

    std::shared_ptr<LoopbackSignalQueue> outSignalQueue_;
    std::shared_ptr<LoopbackSignalQueue> inSignalQueue_;

    mutable int numSendConnections_;
    mutable int numReceiveConnections_;
    std::mutex mutex_;
};
}
}