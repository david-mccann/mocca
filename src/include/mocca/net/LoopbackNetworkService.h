#pragma once

#include <map>

#include "mocca/net/INetworkService.h"
#include "mocca/net/LoopbackConnection.h"

namespace mocca {

template<typename T> class MessageQueue;

namespace net {

class LoopbackNetworkService : public INetworkService {
public:
    static std::string transportStatic() { return "loopback"; }
    std::string transport() const override { return transportStatic(); }
    std::unique_ptr<AbstractConnection> connect(const std::string& queueName) override;
    std::unique_ptr<IConnectionListener> bind(const std::string& queueName) override;

private:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackConnection>>;
    std::map<std::string, std::shared_ptr<LoopbackConnectionQueue>> spawnedConnections_;
};
}
}