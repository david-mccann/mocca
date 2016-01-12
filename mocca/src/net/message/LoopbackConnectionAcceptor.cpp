/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/LoopbackConnectionAcceptor.h"

using namespace mocca::net;

LoopbackConnectionAcceptor::LoopbackConnectionAcceptor(std::shared_ptr<LoopbackConnectionSpawner> spawner)
    : spawner_(spawner) {}

std::unique_ptr<IMessageConnection> LoopbackConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    return spawner_->getServerConnection(timeout);
}