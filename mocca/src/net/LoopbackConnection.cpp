#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/Error.h"
#include "mocca/net/LoopbackConnection.h"
#include "mocca/net/LoopbackNetworkService.h"

namespace mocca {
namespace net {

LoopbackConnection::LoopbackConnection(std::shared_ptr<LoopbackMessageQueue> sendQueue, std::shared_ptr<LoopbackMessageQueue> receiveQueue,
                                       std::shared_ptr<LoopbackSignalQueue> outSignalQueue,
                                       std::shared_ptr<LoopbackSignalQueue> inSignalQueue)
    : identifier_(createIdentifier())
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

mocca::net::LoopbackConnection::~LoopbackConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::string mocca::net::LoopbackConnection::createIdentifier() {
    static unsigned int count = 0;
    ++count;
    return "loopback_" + std::to_string(count);
}

std::string mocca::net::LoopbackConnection::identifier() const {
    return identifier_;
}

void LoopbackConnection::send(ByteArray message) const {
    auto signal = inSignalQueue_->tryDequeue(std::chrono::milliseconds(0));
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", identifier(), __FILE__, __LINE__);
        }
    }

    sendQueue_->enqueue(std::move(message));
}

ByteArray LoopbackConnection::receive(std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->tryDequeue(std::chrono::milliseconds(0));
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", identifier(), __FILE__, __LINE__);
        }
    }

    auto data = receiveQueue_->tryDequeue(timeout);
    return data.isNull() ? ByteArray() : data.release();
}
}
}