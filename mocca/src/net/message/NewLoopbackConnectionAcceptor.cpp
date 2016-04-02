/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/NewLoopbackConnectionAcceptor.h"

using namespace mocca::net;

NewLoopbackConnectionAcceptor::NewLoopbackConnectionAcceptor(std::shared_ptr<NewLoopbackConnectionSpawner> spawner)
    : spawner_(spawner)
    , endpoint_(std::make_shared<Endpoint>("loopback:" + spawner->name())) {}

std::unique_ptr<IMessageConnection> NewLoopbackConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    return spawner_->getServerConnection(timeout);
}

std::shared_ptr<const Endpoint> NewLoopbackConnectionAcceptor::localEndpoint() const {
    return endpoint_;
}