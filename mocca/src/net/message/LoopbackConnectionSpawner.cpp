/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/LoopbackConnectionSpawner.h"

using namespace mocca::net;

LoopbackConnectionSpawner::LoopbackConnectionSpawner(const std::string& name)
    : name_(name) {}

std::unique_ptr<IMessageConnection> LoopbackConnectionSpawner::getClientConnection() {
    auto messageQueue1 = std::make_shared<LoopbackConnection::MQ>();
    auto messageQueue2 = std::make_shared<LoopbackConnection::MQ>();

    auto signalQueue1 = std::make_shared<LoopbackConnection::SQ>();
    auto signalQueue2 = std::make_shared<LoopbackConnection::SQ>();

    static unsigned int connectionCounter = 0;
    Endpoint ep1("loopback", "local", "#" + std::to_string(connectionCounter++));
    Endpoint ep2("loopback", "local", "#" + std::to_string(connectionCounter++));
    std::shared_ptr<const ConnectionID> serverID(std::make_shared<const ConnectionID>(ep1, ep2));
    std::shared_ptr<const ConnectionID> clientID(std::make_shared<const ConnectionID>(ep2, ep1));
    std::unique_ptr<LoopbackConnection> serverConnection(
        new LoopbackConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2, serverID));
    std::unique_ptr<IMessageConnection> clientConnection(
        new LoopbackConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1, clientID));

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