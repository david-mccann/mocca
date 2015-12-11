#pragma once

#include "mocca/base/MessageQueue.h"
#include "mocca/net/stream/Stream.h"

namespace mocca {
namespace net {
class MessageQueueStream : public Stream<MessageQueueStream> {
    friend class Stream<MessageQueueStream>;
public:
    enum class Signal { Disconnect };

    using LoopbackMessageQueue = MessageQueue<unsigned char>;
    using LoopbackSignalQueue = MessageQueue<Signal>;

    MessageQueueStream(std::shared_ptr<LoopbackMessageQueue> sendQueue, std::shared_ptr<LoopbackMessageQueue> receiveQueue,
                       std::shared_ptr<LoopbackSignalQueue> outSignalQueue, std::shared_ptr<LoopbackSignalQueue> inSignalQueue);
    ~MessageQueueStream();

private:
    static std::string createIdentifier();
    std::string identifierImpl() const;
    void writeImpl(ByteArray message, std::chrono::milliseconds timeout) const;
    ByteArray readImpl(uint32_t maxSize, std::chrono::milliseconds timeout) const;

private:
    std::string identifier_;
    std::shared_ptr<LoopbackMessageQueue> sendQueue_;
    std::shared_ptr<LoopbackMessageQueue> receiveQueue_;

    std::shared_ptr<LoopbackSignalQueue> outSignalQueue_;
    std::shared_ptr<LoopbackSignalQueue> inSignalQueue_;

    mutable int numSendConnections_;
    mutable int numReceiveConnections_;
};
}
}