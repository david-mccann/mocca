#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/Error.h"
#include "mocca/net/LoopbackConnection_tmp.h"
#include "mocca/net/LoopbackNetworkService.h"

namespace mocca {
namespace net {

LoopbackConnection_tmp::LoopbackConnection_tmp(std::shared_ptr<LoopbackMessageQueue> sendQueue,
                                               std::shared_ptr<LoopbackMessageQueue> receiveQueue,
                                               std::shared_ptr<LoopbackSignalQueue> outSignalQueue,
                                               std::shared_ptr<LoopbackSignalQueue> inSignalQueue)
    : identifier_(createIdentifier())
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

LoopbackConnection_tmp::~LoopbackConnection_tmp() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::string LoopbackConnection_tmp::createIdentifier() {
    static unsigned int count = 0;
    ++count;
    return "loopback_" + std::to_string(count);
}

std::string LoopbackConnection_tmp::identifier() const {
    return identifier_;
}

void LoopbackConnection_tmp::lock() {
    mutex_.lock();
}

void LoopbackConnection_tmp::unlock() {
    mutex_.unlock();
}

void LoopbackConnection_tmp::send(ByteArray message, std::chrono::milliseconds timeout) const {
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

ByteArray LoopbackConnection_tmp::receive(uint32_t maxSize, std::chrono::milliseconds timeout) const {
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