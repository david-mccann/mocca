/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/base/MessageQueue.h"
#include "mocca/base/Nullable.h"
#include "mocca/base/Thread.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

#include <list>
#include <vector>
#include <set>

namespace mocca {
namespace net {


struct MessageEnvelope {
    MessageEnvelope() {}
    MessageEnvelope(Message msg, std::shared_ptr<const ConnectionID> id);
    MessageEnvelope(MessageEnvelope&& other);
    friend void swap(MessageEnvelope& lhs, MessageEnvelope& rhs);
    MessageEnvelope& operator=(MessageEnvelope other);
    Message message;
    std::shared_ptr<const ConnectionID> connectionID;
};

class ConnectionAggregator : public Runnable {
public:
    enum class DisconnectStrategy { RemoveConnection, ThrowException };

    ConnectionAggregator(std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> connectionAcceptor,
                         DisconnectStrategy disconnectStrategy = DisconnectStrategy::RemoveConnection);
    ~ConnectionAggregator();

    mocca::Nullable<MessageEnvelope> receive(std::chrono::milliseconds timeout);
    void send(MessageEnvelope envelope);

    void interrupt() override;

    bool isLocalMachine(const std::string& machine) const;

private:
    void run() override;

private:
    using EnvelopeQueue = MessageQueue<MessageEnvelope>;

    class ReceiveThread : public Runnable {
    public:
        ReceiveThread(IMessageConnection& connection, EnvelopeQueue& receiveQueue);
        ~ReceiveThread();

    private:
        void run() override;

    private:
        IMessageConnection& connection_;
        EnvelopeQueue& receiveQueue_;
    };

    class SendThread : public Runnable {
    public:
        SendThread(IMessageConnection& connection, EnvelopeQueue& sendQueue);
        ~SendThread();

    private:
        void run() override;

    private:
        IMessageConnection& connection_;
        EnvelopeQueue& sendQueue_;
    };

    struct ThreadedConnection {
        // ctor only needed because of stupid vs2013
        ThreadedConnection(std::unique_ptr<IMessageConnection> connection, const std::thread::id& receiveThreadID,
                           const std::thread::id& sendThreadID)
            : connection(std::move(connection))
            , receiveThreadID(receiveThreadID)
            , sendThreadID(sendThreadID) {}
        std::unique_ptr<IMessageConnection> connection;
        std::thread::id receiveThreadID;
        std::thread::id sendThreadID;
    };

private:
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> connectionAcceptors_;
    DisconnectStrategy disconnectStrategy_;
    EnvelopeQueue sendQueue_;
    EnvelopeQueue receiveQueue_;
    std::list<ThreadedConnection> connections_;
    RunnableGroup runnables_;
    std::set<std::string> localMachineIDs_;
};
}
}