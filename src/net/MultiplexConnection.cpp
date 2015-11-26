#include "mocca/net/MultiplexConnection.h"
#include "mocca/base/ByteArray.h"

using namespace mocca;
using namespace mocca::net;

mocca::net::Multiplexer::Multiplexer(std::unique_ptr<AbstractConnection> connection)
    : connection_(std::move(connection))
    , terminate_(false)
    , thread_(std::thread(&Multiplexer::run, this)) {}

mocca::net::Multiplexer::~Multiplexer() {
    terminate_ = true;
}

std::unique_ptr<AbstractConnection>
mocca::net::Multiplexer::createConnection(const std::string& name) {
    if (!receiveQueues_.count(name)) {
        receiveQueues_[name] = std::make_shared<MessageQueue<ByteArray>>();
    }
    auto receiveQueue = receiveQueues_[name];
    return std::unique_ptr<AbstractConnection>(new MultiplexConnection(name, receiveQueue));
}

void mocca::net::Multiplexer::run() {
    while (!terminate_) {
        auto data = connection_->receive();
        if (!data.isEmpty()) {
            ByteArray packet(std::move(data));
            std::string name = packet.get<std::string>();
//            ByteArray innerData = packet.get<ByteArray>();
//            receiveQueues_[name]->enqueue(std::move(innerData));
        }
    }
}

mocca::net::MultiplexConnection::MultiplexConnection(
    const std::string& name, std::shared_ptr<mocca::MessageQueue<ByteArray>> receiveQueue)
    : name_(name)
    , receiveQueue_(receiveQueue) {}

void mocca::net::MultiplexConnection::send(ByteArray message) const {}

ByteArray mocca::net::MultiplexConnection::receive(std::chrono::milliseconds timeout) const {
    return receiveQueue_->tryDequeue(timeout);
}