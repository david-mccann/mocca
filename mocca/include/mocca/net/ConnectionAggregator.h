#pragma once

#include "mocca/base/MessageQueue.h"
#include "mocca/base/Nullable.h"
#include "mocca/base/Thread.h"
#include "mocca/net/IProtocolConnection.h"
#include "mocca/net/IProtocolConnectionAcceptor.h"

#include <vector>

namespace mocca {
namespace net {


struct MessageEnvelope {
    MessageEnvelope() {}
    MessageEnvelope(mocca::ByteArray msg, std::string id);
    MessageEnvelope(MessageEnvelope&& other);
    mocca::ByteArray message;
    std::string senderID;
};

using EnvelopeQueue = MessageQueue<MessageEnvelope>;

class ConnectionAggregator : public Runnable {
public:
    enum class DisconnectStrategy { RemoveConnection, ThrowException };

    ConnectionAggregator(std::unique_ptr<mocca::net::IProtocolConnectionAcceptor> connectionAcceptor,
                         DisconnectStrategy disconnectStrategy = DisconnectStrategy::ThrowException);
    ~ConnectionAggregator();

    mocca::Nullable<MessageEnvelope> receive(std::chrono::milliseconds timeout);
    void send(MessageEnvelope envelope);

    void interrupt() override;

private:
    void run() override;

private:
    class ReceiveThread : public Runnable {
    public:
        ReceiveThread(IProtocolConnection& connection, EnvelopeQueue& receiveQueue);
        ~ReceiveThread();

    private:
        void run() override;

    private:
        IProtocolConnection& connection_;
        EnvelopeQueue& receiveQueue_;
    };

    class SendThread : public Runnable {
    public:
        SendThread(IProtocolConnection& connection, EnvelopeQueue& sendQueue);
        ~SendThread();

    private:
        void run() override;

    private:
        IProtocolConnection& connection_;
        EnvelopeQueue& sendQueue_;
    };

    struct ThreadedConnection {
        std::unique_ptr<IProtocolConnection> connection;
        std::thread::id receiveThreadID;
        std::thread::id sendThreadID;
    };

private:
    std::unique_ptr<mocca::net::IProtocolConnectionAcceptor> connectionAcceptor_;
    DisconnectStrategy disconnectStrategy_;
    EnvelopeQueue sendQueue_;
    EnvelopeQueue receiveQueue_;
    std::vector<ThreadedConnection> connections_;
    RunnableGroup runnables_;
};
}
}