#pragma once

#include <map>

#include "mocca/net/IPhysicalNetworkService.h"
#include "mocca/testing/LoopbackPhysicalConnection.h"

namespace mocca {

template <typename T> class MessageQueue;

namespace net {

class LoopbackPhysicalNetworkService : public IPhysicalNetworkService {
public:
    static std::string transportStatic() { return "loopback"; }
    std::string transport() const override { return transportStatic(); }
    std::unique_ptr<IPhysicalConnection> connect(const std::string& queueName) override;
    std::unique_ptr<IPhysicalConnectionAcceptor> bind(const std::string& queueName) override;

private:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackPhysicalConnection>>;
    std::map<std::string, std::shared_ptr<LoopbackConnectionQueue>> spawnedConnections_;
};
}
}