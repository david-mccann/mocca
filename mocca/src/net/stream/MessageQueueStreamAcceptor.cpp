#include "mocca/net/stream/MessageQueueStreamAcceptor.h"

using namespace mocca::net;

mocca::net::MessageQueueStreamAcceptor::MessageQueueStreamAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue)
    : connectionQueue_(connectionQueue) {}

std::unique_ptr<MessageQueueStream> mocca::net::MessageQueueStreamAcceptor::acceptImpl(std::chrono::milliseconds timeout) {
    auto connectionNullable = connectionQueue_->tryDequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}