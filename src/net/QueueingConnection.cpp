#include "mocca/net/QueueingConnection.h"

using namespace mocca::net;

MessageEnvelope::MessageEnvelope(mocca::ByteArray msg, std::string id)
    : message(std::move(msg)), senderID(std::move(id)) {}

MessageEnvelope::MessageEnvelope(MessageEnvelope&& other)
    : message(std::move(other.message)), senderID(std::move(other.senderID)) {}

QueueingConnection::QueueingConnection(std::unique_ptr<mocca::net::AbstractConnection> connection,
                                       EnvelopeQueue& sendQueue, EnvelopeQueue& receiveQueue)
    : connection_(std::move(connection)), sendQueue_(sendQueue), receiveQueue_(receiveQueue),
      terminate_(false), sendThread_(std::thread(&QueueingConnection::runSend, this)),
      receiveThread_(std::thread(&QueueingConnection::runReceive, this)) {}

QueueingConnection::~QueueingConnection() {
    terminate_ = true;
    if (sendThread_.joinable()) {
        sendThread_.join();
    }
    if (receiveThread_.joinable()) {
        receiveThread_.join();
    }
}

std::exception_ptr QueueingConnection::currentException()
{
    return currentException_;
}

void QueueingConnection::runReceive() {
    try {
        while (!terminate_) {
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
        while (!terminate_) {
            auto connectionID = connection_->identifier();
            auto dataNullable =
                sendQueue_.tryDequeueFiltered([&connectionID](const MessageEnvelope& envelope) {
                return envelope.senderID == connectionID;
            }, std::chrono::milliseconds(500));
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