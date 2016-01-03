#include "mocca/net/ConnectionAggregator.h"

#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"

using namespace mocca::net;
using namespace mocca;

MessageEnvelope::MessageEnvelope(mocca::ByteArray msg, std::string id)
    : message(std::move(msg))
    , senderID(std::move(id)) {}

MessageEnvelope::MessageEnvelope(MessageEnvelope&& other)
    : message(std::move(other.message))
    , senderID(std::move(other.senderID)) {}


ConnectionAggregator::ConnectionAggregator(std::vector<std::unique_ptr<IMessageConnectionAcceptor>> connectionAcceptors,
                                           DisconnectStrategy disconnectStrategy)
    : connectionAcceptors_(std::move(connectionAcceptors))
    , disconnectStrategy_(disconnectStrategy) {
    start();
}

mocca::net::ConnectionAggregator::~ConnectionAggregator() {
    join();
}

void mocca::net::ConnectionAggregator::interrupt() {
    mocca::Runnable::interrupt();
    runnables_.interruptAll();
}

mocca::Nullable<MessageEnvelope> ConnectionAggregator::receive(std::chrono::milliseconds timeout) {
    rethrowException();
    return receiveQueue_.dequeue(timeout);
}

void ConnectionAggregator::send(MessageEnvelope envelope) {
    rethrowException();
    sendQueue_.enqueue(std::move(envelope));
}

void ConnectionAggregator::run() {
    while (!isInterrupted()) {
        try {
            for (auto& acceptor : connectionAcceptors_) {
                auto connection = acceptor->accept(std::chrono::milliseconds(100));
                if (connection != nullptr) {
                    auto sendRunnable = std::unique_ptr<SendThread>(new SendThread(*connection, sendQueue_));
                    auto receiveRunnable = std::unique_ptr<ReceiveThread>(new ReceiveThread(*connection, receiveQueue_));
                    connections_.emplace_back(std::move(connection), sendRunnable->id(), receiveRunnable->id());
                    runnables_.addRunnable(std::move(sendRunnable));
                    runnables_.addRunnable(std::move(receiveRunnable));
                }
            }

            try {
                runnables_.rethrowException();
            }
            catch (const ConnectionClosedError& err) {
                if (disconnectStrategy_ == DisconnectStrategy::RemoveConnection) {
                    auto it = std::find_if(begin(connections_), end(connections_), [&](const ThreadedConnection& connection) {
                        return connection.connection->identifier() == err.connectionID();
                    });
                    runnables_.removeRunnable(it->receiveThreadID);
                    runnables_.removeRunnable(it->sendThreadID);
                    connections_.erase(it);
                    LDEBUG("Connection to peer has been lost");
                }
                else {
                    throw err;
                }
            }
        } catch (...) {
            setException(std::current_exception());
        }
    }
}


ConnectionAggregator::ReceiveThread::ReceiveThread(IMessageConnection& connection, EnvelopeQueue& receiveQueue)
    : connection_(connection)
    , receiveQueue_(receiveQueue) {
    start();
}

ConnectionAggregator::ReceiveThread::~ReceiveThread() {
    join();
}

void ConnectionAggregator::ReceiveThread::run() {
    try {
        while (!isInterrupted()) {
            auto data = connection_.receive();
            if (!data.isEmpty()) {
                MessageEnvelope envelope(std::move(data), connection_.identifier());
                receiveQueue_.enqueue(std::move(envelope));
            }
        }
    } catch (...) {
        setException(std::current_exception());
    }
}


ConnectionAggregator::SendThread::SendThread(IMessageConnection& connection, EnvelopeQueue& sendQueue)
    : connection_(connection)
    , sendQueue_(sendQueue) {
    start();
}

mocca::net::ConnectionAggregator::SendThread::~SendThread() {
    join();
}

void ConnectionAggregator::SendThread::run() {
    try {
        while (!isInterrupted()) {
            auto connectionID = connection_.identifier();
            auto dataNullable = sendQueue_.dequeueFiltered(
                [&connectionID](const MessageEnvelope& envelope) { return envelope.senderID == connectionID; },
                std::chrono::milliseconds(100));
            if (!dataNullable.isNull()) {
                auto data = dataNullable.release();
                connection_.send(std::move(data.message));
            }
        }
    } catch (...) {
        setException(std::current_exception());
    }
}