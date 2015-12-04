#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/IProtocolConnection.h"
#include "mocca/base/Thread.h"

#include <string>

namespace mocca {
namespace net {

struct MessageEnvelope {
    MessageEnvelope() {}
    MessageEnvelope(mocca::ByteArray msg, std::string id);
    MessageEnvelope(MessageEnvelope&& other);
    mocca::ByteArray message;
    std::string senderID;
};

using EnvelopeQueue = mocca::MessageQueue<MessageEnvelope>;

class QueueingConnection {
public:
    QueueingConnection(std::unique_ptr<mocca::net::IProtocolConnection> connection,
                       EnvelopeQueue& sendQueue, EnvelopeQueue& receiveQueue);

    std::exception_ptr currentException();

private:
    void runSend();
    void runReceive();

private:
    std::unique_ptr<mocca::net::IProtocolConnection> connection_;
    EnvelopeQueue& sendQueue_;
    EnvelopeQueue& receiveQueue_;
    Thread sendThread_;
    Thread receiveThread_;
    std::exception_ptr currentException_;
};
}
}