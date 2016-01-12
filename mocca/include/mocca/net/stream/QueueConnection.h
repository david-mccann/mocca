/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/base/MessageQueue.h"
#include "mocca/net/IStreamConnection.h"

namespace mocca {
namespace net {
class QueueConnection : public IStreamConnection {
public:
    enum class Signal { Disconnect };
    using MQ = MessageQueue<unsigned char>;
    using SQ = MessageQueue<Signal>;

    QueueConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                    std::shared_ptr<SQ> inSignalQueue, std::shared_ptr<const ConnectionID> connectionID);
    ~QueueConnection();

    std::shared_ptr<const ConnectionID> connectionID() const override;
    bool isConnected() const override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;

private:
    ByteArray readFromStream(uint32_t maxSize, std::chrono::milliseconds timeout) const override;

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