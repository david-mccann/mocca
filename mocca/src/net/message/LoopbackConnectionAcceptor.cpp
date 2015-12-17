#include "mocca/net/message/LoopbackConnectionAcceptor.h"

using namespace mocca::net;

LoopbackConnectionAcceptor::LoopbackConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue)
    : connectionQueue_(connectionQueue) {}

std::unique_ptr<IMessageConnection> LoopbackConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    auto connectionNullable = connectionQueue_->tryDequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}