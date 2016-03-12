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

void QueueConnection::send(ByteArray message) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
    }
    auto messageData = message.data();
    auto messageDataEnd = message.data() + message.size();
    while (messageData != messageDataEnd) {
        sendQueue_->enqueue(*messageData);
        ++messageData;
    }
}

ByteArray QueueConnection::readFromStream(uint32_t maxSize, std::chrono::milliseconds timeout) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
    }
    ByteArray result;
    while (result.size() < maxSize) {
        auto dataNullable = receiveQueue_->dequeue(timeout);
        if (dataNullable.isNull()) {
            return result;
        }
        auto data = dataNullable.get();
        result.append(&data, sizeof(unsigned char));
    }
    return result;
}
