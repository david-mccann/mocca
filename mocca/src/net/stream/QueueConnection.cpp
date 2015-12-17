#include "mocca/net/stream/QueueConnection.h"

#include "mocca/net/NetworkError.h"


using namespace mocca;
using namespace mocca::net;

QueueConnection::QueueConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                                 std::shared_ptr<SQ> inSignalQueue)
    : identifier_(createIdentifier())
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

QueueConnection::~QueueConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::string QueueConnection::createIdentifier() {
    static unsigned int count = 0;
    ++count;
    return "queue_" + std::to_string(count);
}

std::string QueueConnection::identifier() const {
    return identifier_;
}

void QueueConnection::send(ByteArray message, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->dequeueNoWait();
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

ByteArray QueueConnection::receive(uint32_t maxSize, std::chrono::milliseconds timeout) const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull()) {
        if (signal == Signal::Disconnect) {
            throw ConnectionClosedError("Connection to peer has been closed", identifier_, __FILE__, __LINE__);
        }
    }
    ByteArray result;
    while (result.size() < maxSize) {
        auto dataNullable = receiveQueue_->dequeue(timeout);
        if (dataNullable.isNull()) {
            return result;
        }
        auto data = dataNullable.get();
        result.append(&data, sizeof(unsigned char));
    }
    return result;
}
