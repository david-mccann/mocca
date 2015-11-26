#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/AbstractConnection.h"

#include <string>
#include <atomic>
#include <thread>

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
    QueueingConnection(std::unique_ptr<mocca::net::AbstractConnection> connection,
                       EnvelopeQueue& sendQueue, EnvelopeQueue& receiveQueue);
    ~QueueingConnection();

    std::exception_ptr currentException();

private:
    void runSend();
    void runReceive();

private:
    std::unique_ptr<mocca::net::AbstractConnection> connection_;
    EnvelopeQueue& sendQueue_;
    EnvelopeQueue& receiveQueue_;
    std::atomic<bool> terminate_;
    std::thread sendThread_;
    std::thread receiveThread_;
    std::exception_ptr currentException_;
};
}
}