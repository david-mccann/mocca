#include "mocca/net/message/LoopbackConnectionSpawner.h"

using namespace mocca::net;

LoopbackConnectionSpawner::LoopbackConnectionSpawner(const std::string& name)
    : name_(name) {}

std::unique_ptr<IMessageConnection> LoopbackConnectionSpawner::getClientConnection() {
    auto messageQueue1 = std::make_shared<LoopbackConnection::MQ>();
    auto messageQueue2 = std::make_shared<LoopbackConnection::MQ>();

    auto signalQueue1 = std::make_shared<LoopbackConnection::SQ>();
    auto signalQueue2 = std::make_shared<LoopbackConnection::SQ>();

    std::unique_ptr<LoopbackConnection> serverConnection(new LoopbackConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2));
    std::unique_ptr<IMessageConnection> clientConnection(new LoopbackConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1));

    serverConnections_.enqueue(std::move(serverConnection));
    return clientConnection;
}

std::unique_ptr<IMessageConnection> LoopbackConnectionSpawner::getServerConnection(std::chrono::milliseconds timeout) {
    auto connectionNullable = serverConnections_.dequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}

std::string mocca::net::LoopbackConnectionSpawner::name() const {
    return name_;
}