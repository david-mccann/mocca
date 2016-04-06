/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/stream/QueueConnection.h"

#include "mocca/net/NetworkError.h"


using namespace mocca;
using namespace mocca::net;

QueueConnection::QueueConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                                 std::shared_ptr<SQ> inSignalQueue, std::shared_ptr<const ConnectionID> connectionID)
    : connectionID_(connectionID)
    , connected_(true)
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

QueueConnection::~QueueConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::shared_ptr<const ConnectionID> QueueConnection::connectionID() const {
    return connectionID_;
}

bool QueueConnection::isConnected() const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull() && signal == Signal::Disconnect) {
        connected_ = false;
    }
    return connected_;
}

void QueueConnection::send(const uint8_t* data, uint32_t size) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
    }
    auto dataIt = data;
    auto dataEnd = data + size;
    while (dataIt != dataEnd) {
        sendQueue_->enqueue(*dataIt);
        ++dataIt;
    }
}

uint32_t QueueConnection::readFromStream(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
    }
    for (uint32_t i = 0; i < maxSize; ++i) {
        auto dataNullable = receiveQueue_->dequeue(timeout);
        if (dataNullable.isNull()) {
            return i;
        }
        auto data = dataNullable.get();
        buffer[i] = data;
    }
    return maxSize;
}
