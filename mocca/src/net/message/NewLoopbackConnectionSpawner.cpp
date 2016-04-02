/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/NewLoopbackConnectionSpawner.h"

using namespace mocca::net;

NewLoopbackConnectionSpawner::NewLoopbackConnectionSpawner(const std::string& name)
    : name_(name) {}

std::unique_ptr<IMessageConnection> NewLoopbackConnectionSpawner::getClientConnection() {
    auto messageQueue1 = std::make_shared<NewLoopbackConnection::MQ>();
    auto messageQueue2 = std::make_shared<NewLoopbackConnection::MQ>();

    auto signalQueue1 = std::make_shared<NewLoopbackConnection::SQ>();
    auto signalQueue2 = std::make_shared<NewLoopbackConnection::SQ>();

    static unsigned int connectionCounter = 0;
    Endpoint ep1("newloopback", "local", "#" + std::to_string(connectionCounter++));
    Endpoint ep2("newloopback", "local", "#" + std::to_string(connectionCounter++));
    std::shared_ptr<const ConnectionID> serverID(std::make_shared<const ConnectionID>(ep1, ep2));
    std::shared_ptr<const ConnectionID> clientID(std::make_shared<const ConnectionID>(ep2, ep1));
    std::unique_ptr<NewLoopbackConnection> serverConnection(
        new NewLoopbackConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2, serverID));
    std::unique_ptr<IMessageConnection> clientConnection(
        new NewLoopbackConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1, clientID));

    serverConnections_.enqueue(std::move(serverConnection));
    return clientConnection;
}

std::unique_ptr<IMessageConnection> NewLoopbackConnectionSpawner::getServerConnection(std::chrono::milliseconds timeout) {
    auto connectionNullable = serverConnections_.dequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}

std::string mocca::net::NewLoopbackConnectionSpawner::name() const {
    return name_;
}