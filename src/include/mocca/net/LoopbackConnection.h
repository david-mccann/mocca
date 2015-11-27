#pragma once

#include "mocca/net/IPhysicalConnection.h"

namespace mocca {

template <typename T> class MessageQueue;
class ByteArray;

namespace net {

class LoopbackConnection : public IPhysicalConnection {
public:
    enum class Signal { Disconnect };

    using LoopbackMessageQueue = MessageQueue<ByteArray>;
    using LoopbackSignalQueue = MessageQueue<Signal>;

    LoopbackConnection(std::shared_ptr<LoopbackMessageQueue> sendQueue,
                       std::shared_ptr<LoopbackMessageQueue> receiveQueue,
                       std::shared_ptr<LoopbackSignalQueue> outSignalQueue,
                       std::shared_ptr<LoopbackSignalQueue> inSignalQueue);
    ~LoopbackConnection();

    std::string identifier() const override;
    void send(ByteArray message) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

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
};
}
}