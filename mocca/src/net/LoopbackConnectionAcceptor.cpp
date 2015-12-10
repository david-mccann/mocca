#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/Error.h"
#include "mocca/net/LoopbackConnection.h"
#include "mocca/net/LoopbackConnectionAcceptor.h"
#include "mocca/net/LoopbackNetworkService.h"

namespace mocca {
namespace net {

LoopbackConnectionAcceptor::LoopbackConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue)
    : connectionQueue_(connectionQueue) {}

std::unique_ptr<IProtocolConnection> LoopbackConnectionAcceptor::getConnection(std::chrono::milliseconds timeout) {
    auto connectionNullable = connectionQueue_->tryDequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}
}
}