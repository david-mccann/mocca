#pragma once

#include "mocca/base/Nullable.h"
#include "mocca/net/QueueingConnection.h"
#include "mocca/net/IConnectionListener.h"

#include <thread>
#include <vector>
#include <atomic>

namespace mocca {
namespace net {

class ConnectionAggregator {
public:
    enum class DisconnectStrategy { RemoveConnection, ThrowException };

    ConnectionAggregator(
        std::unique_ptr<mocca::net::IConnectionListener> connectionListener,
        DisconnectStrategy disconnectStrategy = DisconnectStrategy::ThrowException);
    ~ConnectionAggregator();

    mocca::Nullable<MessageEnvelope> receive(std::chrono::milliseconds timeout);
    void send(MessageEnvelope envelope);

private:
    void runListen();
    void checkConnectionExceptions();

private:
    DisconnectStrategy disconnectStrategy_;
    EnvelopeQueue sendQueue_;
    EnvelopeQueue receiveQueue_;
    std::list<QueueingConnection> connections_;
    std::atomic<bool> terminate_;
    std::unique_ptr<mocca::net::IConnectionListener> connectionListener_;
    std::thread listenerThread_;
    std::exception_ptr currentException_;
};
}
}