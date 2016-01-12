/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/LoopbackConnectionFactory.h"

#include "mocca/net/NetworkError.h"
#include "mocca/net/message/LoopbackConnectionAcceptor.h"

using namespace mocca::net;


std::unique_ptr<IMessageConnection> LoopbackConnectionFactory::connect(const std::string& name) {
    auto spawner = getSpawner(name);
    if (spawner == nullptr) {
        throw NetworkError("No connection acceptor bound to name " + name, __FILE__, __LINE__);
    }
    return spawner->getClientConnection();
}

std::unique_ptr<IMessageConnectionAcceptor> LoopbackConnectionFactory::bind(const std::string& name) {
    auto spawner = getSpawner(name);
    if (spawner == nullptr) {
        spawner = std::make_shared<LoopbackConnectionSpawner>(name);
        spawners_.push_back(spawner);
    }
    return std::unique_ptr<IMessageConnectionAcceptor>(new LoopbackConnectionAcceptor(spawner));
}

std::shared_ptr<LoopbackConnectionSpawner> mocca::net::LoopbackConnectionFactory::getSpawner(const std::string& name) {
    for (const auto& spawner : spawners_) {
        if (spawner->name() == name) {
            return spawner;
        }
    }
    return nullptr;
}