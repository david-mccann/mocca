#include "mocca/net/message/LoopbackConnection.h"

#include "mocca/net/NetworkError.h"

using namespace mocca;
using namespace mocca::net;

LoopbackConnection::LoopbackConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                                       std::shared_ptr<SQ> inSignalQueue)
    : identifier_(createIdentifier())
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

LoopbackConnection::~LoopbackConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::string LoopbackConnection::createIdentifier() {
    static unsigned int count = 0;
    ++count;
    return "loopback_" + std::to_string(count);
}

std::string LoopbackConnection::identifier() const {
    return identifier_;
}

void LoopbackConnection::send(ByteArray message, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", identifier_, __FILE__, __LINE__);
        }
    }

    sendQueue_->enqueue(std::move(message));
}

ByteArray LoopbackConnection::receive(std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", identifier_, __FILE__, __LINE__);
        }
    }

    auto data = receiveQueue_->dequeue(timeout);
    return data.isNull() ? ByteArray() : data.release();
}