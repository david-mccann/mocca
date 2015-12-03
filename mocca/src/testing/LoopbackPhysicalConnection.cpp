#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/Error.h"
#include "mocca/testing/LoopbackPhysicalConnection.h"
#include "mocca/net/LoopbackNetworkService.h"

namespace mocca {
namespace net {

LoopbackPhysicalConnection::LoopbackPhysicalConnection(std::shared_ptr<LoopbackMessageQueue> sendQueue,
                                               std::shared_ptr<LoopbackMessageQueue> receiveQueue,
                                               std::shared_ptr<LoopbackSignalQueue> outSignalQueue,
                                               std::shared_ptr<LoopbackSignalQueue> inSignalQueue)
    : identifier_(createIdentifier())
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

LoopbackPhysicalConnection::~LoopbackPhysicalConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::string LoopbackPhysicalConnection::createIdentifier() {
    static unsigned int count = 0;
    ++count;
    return "loopback_" + std::to_string(count);
}

std::string LoopbackPhysicalConnection::identifier() const {
    return identifier_;
}

void LoopbackPhysicalConnection::lock() {
    mutex_.lock();
}

void LoopbackPhysicalConnection::unlock() {
    mutex_.unlock();
}

void LoopbackPhysicalConnection::send(ByteArray message, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->tryDequeue(std::chrono::milliseconds(0));
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", __FILE__, __LINE__);
        }
    }
    auto messageData = message.data();
    auto messageDataEnd = message.data() + message.size();
    while (messageData != messageDataEnd) {
        sendQueue_->enqueue(*messageData);
        ++messageData;
    }
}

ByteArray LoopbackPhysicalConnection::receive(uint32_t maxSize, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->tryDequeue(std::chrono::milliseconds(0));
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", __FILE__, __LINE__);
        }
    }
    ByteArray result;
    while (result.size() < maxSize) {
        auto dataNullable = receiveQueue_->tryDequeue(timeout);
        if (dataNullable.isNull()) {
            return result;
        }
        auto data = dataNullable.get();
        result.append(&data, sizeof(unsigned char));
    }
    return result;
}
}
}