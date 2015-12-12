#include "mocca/net/stream/QueueConnectionAcceptor.h"

using namespace mocca::net;

QueueConnectionAcceptor::QueueConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue)
    : connectionQueue_(connectionQueue) {}

std::unique_ptr<IStreamConnection> QueueConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    auto connectionNullable = connectionQueue_->tryDequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}