#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/Error.h"
#include "mocca/log/LogManager.h"

using namespace mocca::net;

ConnectionAggregator::ConnectionAggregator(
    std::unique_ptr<mocca::net::IConnectionListener> connectionListener,
    DisconnectStrategy disconnectStrategy)
    : disconnectStrategy_(disconnectStrategy)
    , terminate_(false)
    , connectionListener_(std::move(connectionListener))
    , listenerThread_(std::thread(&ConnectionAggregator::runListen, this)) {}

ConnectionAggregator::~ConnectionAggregator() {
    terminate_ = true;
    if (listenerThread_.joinable()) {
        listenerThread_.join();
    }
    connections_.clear();
}

mocca::Nullable<MessageEnvelope> ConnectionAggregator::receive(std::chrono::milliseconds timeout) {
    if (currentException_ != nullptr) {
        std::rethrow_exception(currentException_);
    }
    return receiveQueue_.tryDequeue(timeout);
}

void ConnectionAggregator::send(MessageEnvelope envelope) {
    if (currentException_ != nullptr) {
        std::rethrow_exception(currentException_);
    }
    sendQueue_.enqueue(std::move(envelope));
}

void ConnectionAggregator::runListen() {
    while (!terminate_) {
        checkConnectionExceptions();
        auto connection = connectionListener_->getConnection(std::chrono::milliseconds(500));
        if (connection != nullptr) {
            connections_.emplace_back(std::move(connection), sendQueue_, receiveQueue_);
        }
    }
}

void mocca::net::ConnectionAggregator::checkConnectionExceptions() {
    auto it = begin(connections_);
    while (it != end(connections_)) {
        auto ex = it->currentException();
        if (ex != nullptr) {
            try {
                std::rethrow_exception(ex);
            } catch (const ConnectionClosedError&) {
                if (disconnectStrategy_ == DisconnectStrategy::RemoveConnection) {
                    it = connections_.erase(it);
                    currentException_ = nullptr;
                    LDEBUG("Connection to peer has been lost");
                    continue;
                } else {
                    currentException_ = std::current_exception();
                }
            } catch (...) {
                currentException_ = std::current_exception();
            }
        }
        ++it;
    }
}