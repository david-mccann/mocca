#include "mocca/base/MessageQueue.h"
#include "mocca/net/LoopbackConnectionAcceptor_tmp.h"
#include "mocca/net/LoopbackConnection_tmp.h"

namespace mocca {
namespace net {

LoopbackConnectionAcceptor_tmp::LoopbackConnectionAcceptor_tmp(std::shared_ptr<LoopbackConnectionQueue> connectionQueue)
    : connectionQueue_(connectionQueue) {}

std::unique_ptr<IPhysicalConnection> LoopbackConnectionAcceptor_tmp::getConnection(std::chrono::milliseconds timeout) {
    auto connectionNullable = connectionQueue_->tryDequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}
}
}