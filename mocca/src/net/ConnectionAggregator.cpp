/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include "mocca/net/ConnectionAggregator.h"

#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"

using namespace mocca::net;
using namespace mocca;

MessageEnvelope::MessageEnvelope(Message msg, std::shared_ptr<const ConnectionID> id)
    : message(std::move(msg))
    , connectionID(id) {}

MessageEnvelope::MessageEnvelope(MessageEnvelope&& other)
    : message(std::move(other.message))
    , connectionID(std::move(other.connectionID)) {}

namespace mocca {
namespace net {
void swap(MessageEnvelope& lhs, MessageEnvelope& rhs) {
    using std::swap;
    swap(lhs.connectionID, rhs.connectionID);
    swap(lhs.message, rhs.message);
}
}
}

MessageEnvelope& mocca::net::MessageEnvelope::operator=(MessageEnvelope other) {
    swap(other, *this);
    return *this;
}

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

                    auto localMachine = connection->connectionID()->localEndpoint.machine;
                    localMachineIDs_.insert(localMachine);

                    connections_.emplace_back(std::move(connection), sendRunnable->id(), receiveRunnable->id());
                    runnables_.addRunnable(std::move(sendRunnable));
                    runnables_.addRunnable(std::move(receiveRunnable));                    
                }
            }

            try {
                runnables_.rethrowException();
            } catch (const ConnectionClosedError& err) {
                LDEBUG("Connection to peer " << err.connectionID().peerEndpoint << " has been lost");
                if (disconnectStrategy_ == DisconnectStrategy::RemoveConnection) {
                    auto it = std::find_if(begin(connections_), end(connections_), [&](const ThreadedConnection& connection) {
                        return *connection.connection->connectionID() == err.connectionID();
                    });
                    runnables_.removeRunnable(it->receiveThreadID);
                    runnables_.removeRunnable(it->sendThreadID);
                    connections_.erase(it);
                } else {
                    throw err;
                }
            }
        } catch (...) {
            setException(std::current_exception());
        }
    }
}

bool ConnectionAggregator::isLocalMachine(const std::string& machine) const {
    return localMachineIDs_.count(machine) != 0;
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
            if (!data.empty()) {
                MessageEnvelope envelope(std::move(data), connection_.connectionID());
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
            auto connectionID = connection_.connectionID();
            auto dataNullable = sendQueue_.dequeueFiltered(
                [&connectionID](const MessageEnvelope& envelope) { return envelope.connectionID == connectionID; },
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