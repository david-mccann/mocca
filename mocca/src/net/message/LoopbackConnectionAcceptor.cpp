#include "mocca/net/message/LoopbackConnectionAcceptor.h"

using namespace mocca::net;

LoopbackConnectionAcceptor::LoopbackConnectionAcceptor(std::shared_ptr<LoopbackConnectionSpawner> spawner)
    : spawner_(spawner) {}

std::string mocca::net::LoopbackConnectionAcceptor::protocol() const {
    return "loopback";
}

Endpoint mocca::net::LoopbackConnectionAcceptor::connectionEndpoint() const {
    return Endpoint(protocol(), spawner_->name());
}

std::unique_ptr<IMessageConnection> LoopbackConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    return spawner_->getServerConnection(timeout);
}