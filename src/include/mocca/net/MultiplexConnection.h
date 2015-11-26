#pragma once

#include "mocca/net/AbstractConnection.h"
#include "mocca/base/Thread.h"
#include "mocca/base/MessageQueue.h"

#include <unordered_map>
#include <atomic>

namespace mocca {
namespace net {

class Multiplexer {
public:
    Multiplexer(std::unique_ptr<AbstractConnection> connection);
    ~Multiplexer();

    std::unique_ptr<AbstractConnection> createConnection(const std::string& name);

private:
    void run();

private:
    std::unique_ptr<AbstractConnection> connection_;
    std::atomic<bool> terminate_;
    mocca::Thread thread_;
    std::unordered_map<std::string, std::shared_ptr<mocca::MessageQueue<ByteArray>>> receiveQueues_;
};


class MultiplexConnection : public AbstractConnection {
public:
    MultiplexConnection(const std::string& name, std::shared_ptr<mocca::MessageQueue<ByteArray>> receiveQueue);
    virtual ~MultiplexConnection() {}

    virtual void send(ByteArray message) const;
    virtual ByteArray receive(std::chrono::milliseconds timeout = std::chrono::milliseconds(500)) const;

private:
    std::string name_;
    std::shared_ptr<mocca::MessageQueue<ByteArray>> receiveQueue_;
};

}
}