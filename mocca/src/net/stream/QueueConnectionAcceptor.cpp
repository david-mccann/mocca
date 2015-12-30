#include "mocca/net/stream/QueueConnectionAcceptor.h"

using namespace mocca::net;

QueueConnectionAcceptor::QueueConnectionAcceptor(std::shared_ptr<QueueConnectionSpawner> spawner)
    : spawner_(spawner) {}

std::unique_ptr<IStreamConnection> QueueConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    return spawner_->getServerConnection(timeout);
}