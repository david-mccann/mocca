#include "mocca/net/stream/MessageQueueStream.h"

#include "mocca/net/Error.h"

using namespace mocca;
using namespace mocca::net;

MessageQueueStream::MessageQueueStream(std::shared_ptr<LoopbackMessageQueue> sendQueue, std::shared_ptr<LoopbackMessageQueue> receiveQueue,
                                       std::shared_ptr<LoopbackSignalQueue> outSignalQueue,
                                       std::shared_ptr<LoopbackSignalQueue> inSignalQueue)
    : identifier_(createIdentifier())
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

MessageQueueStream::~MessageQueueStream() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::string MessageQueueStream::createIdentifier() {
    static unsigned int count = 0;
    ++count;
    return "loopback_" + std::to_string(count);
}

std::string MessageQueueStream::identifierImpl() const {
    return identifier_;
}

void MessageQueueStream::writeImpl(ByteArray message, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->tryDequeue(std::chrono::milliseconds(0));
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", identifier_, __FILE__, __LINE__);
        }
    }
    auto messageData = message.data();
    auto messageDataEnd = message.data() + message.size();
    while (messageData != messageDataEnd) {
        sendQueue_->enqueue(*messageData);
        ++messageData;
    }
}

ByteArray MessageQueueStream::readImpl(uint32_t maxSize, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->tryDequeue(std::chrono::milliseconds(0));
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", identifier_, __FILE__, __LINE__);
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
