#include "mocca/net/message/LoopbackConnection.h"

#include "mocca/net/NetworkError.h"

using namespace mocca;
using namespace mocca::net;

LoopbackConnection::LoopbackConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                                       std::shared_ptr<SQ> inSignalQueue, std::shared_ptr<const ConnectionID> connectionID)
    : connectionID_(connectionID)
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

LoopbackConnection::~LoopbackConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::shared_ptr<const ConnectionID> LoopbackConnection::connectionID() const {
    return connectionID_;
}

void LoopbackConnection::send(ByteArray message, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
        }
    }

    sendQueue_->enqueue(std::move(message));
}

ByteArray LoopbackConnection::receive(std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
        }
    }

    auto data = receiveQueue_->dequeue(timeout);
    return data.isNull() ? ByteArray() : data.release();
}