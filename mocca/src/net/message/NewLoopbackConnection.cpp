/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/NewLoopbackConnection.h"

#include "mocca/net/NetworkError.h"

using namespace mocca;
using namespace mocca::net;

NewLoopbackConnection::NewLoopbackConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue,
                                             std::shared_ptr<SQ> outSignalQueue, std::shared_ptr<SQ> inSignalQueue,
                                             std::shared_ptr<const ConnectionID> connectionID)
    : connectionID_(connectionID)
    , connected_(true)
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

NewLoopbackConnection::~NewLoopbackConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::shared_ptr<const ConnectionID> NewLoopbackConnection::connectionID() const {
    return connectionID_;
}

bool NewLoopbackConnection::isConnected() const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull() && signal == Signal::Disconnect) {
        connected_ = false;
    }
    return connected_;
}

void NewLoopbackConnection::send(ByteArray message) const {
    throw Error("Not implemented: Use sendNew", __FILE__, __LINE__);
}

ByteArray NewLoopbackConnection::receive(std::chrono::milliseconds timeout) const {
    throw Error("Not implemented: Use receiveNew", __FILE__, __LINE__);
}

void NewLoopbackConnection::sendNew(Message message) {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer " + connectionID_->peerEndpoint.toString() + " has been closed", *connectionID_,
                                    __FILE__, __LINE__);
    }
    sendQueue_->enqueue(std::move(message));
}

Message NewLoopbackConnection::receiveNew(std::chrono::milliseconds timeout) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer " + connectionID_->peerEndpoint.toString() + " has been closed", *connectionID_,
                                    __FILE__, __LINE__);
    }
    auto message = receiveQueue_->dequeue(timeout);
    return message.isNull() ? Message() : message.release();
}