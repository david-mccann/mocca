#include "mocca/net/QueueingConnection.h"

using namespace mocca::net;

MessageEnvelope::MessageEnvelope(mocca::ByteArray msg, std::string id)
    : message(std::move(msg)), senderID(std::move(id)) {}

MessageEnvelope::MessageEnvelope(MessageEnvelope&& other)
    : message(std::move(other.message)), senderID(std::move(other.senderID)) {}

QueueingConnection::QueueingConnection(std::unique_ptr<mocca::net::IProtocolConnection> connection,
                                       EnvelopeQueue& sendQueue, EnvelopeQueue& receiveQueue)
    : connection_(std::move(connection)), sendQueue_(sendQueue), receiveQueue_(receiveQueue),
      sendThread_(&QueueingConnection::runSend, this),
      receiveThread_(&QueueingConnection::runReceive, this) {}

std::exception_ptr QueueingConnection::currentException()
{
    return currentException_;
}

void QueueingConnection::runReceive() {
    try {
        while (!receiveThread_.isInterrupted()) {
            auto data = connection_->receive();
            if (!data.isEmpty()) {
                MessageEnvelope envelope(std::move(data), connection_->identifier());
                receiveQueue_.enqueue(std::move(envelope));
            }
        }
    }
    catch (...) {
        currentException_ = std::current_exception();
    }
}

void QueueingConnection::runSend() {
    try {
        while (!sendThread_.isInterrupted()) {
            auto connectionID = connection_->identifier();
            auto dataNullable =
                sendQueue_.tryDequeueFiltered([&connectionID](const MessageEnvelope& envelope) {
                return envelope.senderID == connectionID;
            }, std::chrono::milliseconds(200));
            if (!dataNullable.isNull()) {
                auto data = dataNullable.release();
                connection_->send(std::move(data.message));
            }
        }
    }
    catch (...) {
        currentException_ = std::current_exception();
    }
}