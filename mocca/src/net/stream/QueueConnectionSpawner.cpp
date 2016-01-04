#include "mocca/net/stream/QueueConnectionSpawner.h"

using namespace mocca::net;

QueueConnectionSpawner::QueueConnectionSpawner(const std::string& name)
    : name_(name) {}

std::unique_ptr<IStreamConnection> QueueConnectionSpawner::getClientConnection() {
    auto messageQueue1 = std::make_shared<QueueConnection::MQ>();
    auto messageQueue2 = std::make_shared<QueueConnection::MQ>();

    auto signalQueue1 = std::make_shared<QueueConnection::SQ>();
    auto signalQueue2 = std::make_shared<QueueConnection::SQ>();

    static unsigned int connectionCounter = 0;
    Endpoint ep1("queue", "local", "#" + std::to_string(connectionCounter++));
    Endpoint ep2("queue", "local", "#" + std::to_string(connectionCounter++));
    std::shared_ptr<const ConnectionID> serverID(std::make_shared<const ConnectionID>(ep1, ep2));
    std::shared_ptr<const ConnectionID> clientID(std::make_shared<const ConnectionID>(ep2, ep1));
    std::unique_ptr<QueueConnection> serverConnection(new QueueConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2, serverID));
    std::unique_ptr<IStreamConnection> clientConnection(new QueueConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1, clientID));

    serverConnections_.enqueue(std::move(serverConnection));
    return clientConnection;
}

std::unique_ptr<IStreamConnection> QueueConnectionSpawner::getServerConnection(std::chrono::milliseconds timeout) {
    auto connectionNullable = serverConnections_.dequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}

std::string mocca::net::QueueConnectionSpawner::name() const {
    return name_;
}