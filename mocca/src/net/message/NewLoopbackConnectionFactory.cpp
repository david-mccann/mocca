/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/NewLoopbackConnectionFactory.h"

#include "mocca/net/NetworkError.h"
#include "mocca/net/message/NewLoopbackConnectionAcceptor.h"

using namespace mocca::net;


std::unique_ptr<IMessageConnection> NewLoopbackConnectionFactory::connect(const std::string& name) {
    auto spawner = getSpawner(name);
    if (spawner == nullptr) {
        throw NetworkError("No connection acceptor bound to name " + name, __FILE__, __LINE__);
    }
    return spawner->getClientConnection();
}

std::unique_ptr<IMessageConnectionAcceptor> NewLoopbackConnectionFactory::bind(const std::string& machine, const std::string& port) {
    static int autoPortCount = 0;
    std::string name = machine + ":" + (port == Endpoint::autoPort() ? std::to_string(autoPortCount++) : port);
    auto spawner = getSpawner(name);
    if (spawner == nullptr) {
        spawner = std::make_shared<NewLoopbackConnectionSpawner>(name);
        spawners_.push_back(spawner);
    }
    return std::unique_ptr<IMessageConnectionAcceptor>(new NewLoopbackConnectionAcceptor(spawner));
}

std::shared_ptr<NewLoopbackConnectionSpawner> mocca::net::NewLoopbackConnectionFactory::getSpawner(const std::string& name) {
    for (const auto& spawner : spawners_) {
        if (spawner->name() == name) {
            return spawner;
        }
    }
    return nullptr;
}