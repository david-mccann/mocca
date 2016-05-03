/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/stream/QueueConnectionAcceptor.h"

using namespace mocca::net;

QueueConnectionAcceptor::QueueConnectionAcceptor(std::shared_ptr<QueueConnectionSpawner> spawner)
    : spawner_(spawner)
    , endpoint_(std::make_shared<Endpoint>("queue:" + spawner->name())) {}

std::unique_ptr<IStreamConnection> QueueConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    return spawner_->getServerConnection(timeout);
}

std::shared_ptr<const Endpoint> mocca::net::QueueConnectionAcceptor::localEndpoint() const {
    return endpoint_;
}