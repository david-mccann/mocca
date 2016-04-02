/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/base/MessageQueue.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/Message.h"

namespace mocca {
namespace net {
class NewLoopbackConnection : public IMessageConnection {
public:
    enum class Signal { Disconnect };
    using MQ = MessageQueue<Message>;
    using SQ = MessageQueue<Signal>;

    NewLoopbackConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                          std::shared_ptr<SQ> inSignalQueue, std::shared_ptr<const ConnectionID> connectionID);
    ~NewLoopbackConnection();

    std::shared_ptr<const ConnectionID> connectionID() const override;
    bool isConnected() const override;

    // USE!!!
    void sendNew(Message message);
    Message receiveNew(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const;

    // DO NOT USE!!!
    void send(ByteArray message) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    std::shared_ptr<const ConnectionID> connectionID_;
    mutable bool connected_;
    std::shared_ptr<MQ> sendQueue_;
    std::shared_ptr<MQ> receiveQueue_;

    std::shared_ptr<SQ> outSignalQueue_;
    std::shared_ptr<SQ> inSignalQueue_;
};
}
}