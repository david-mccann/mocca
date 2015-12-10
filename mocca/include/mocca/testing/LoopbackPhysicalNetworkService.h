#pragma once

#include <map>

#include "mocca/testing/LoopbackPhysicalConnection.h"
#include "mocca/net/IPhysicalConnectionAcceptor.h"

namespace mocca {

template <typename T> class MessageQueue;

namespace net {

class LoopbackPhysicalNetworkService {
public:
    static std::string transportStatic();
    std::string transport() const;
    std::unique_ptr<IPhysicalConnection> connect(const std::string& queueName);
    std::unique_ptr<IPhysicalConnectionAcceptor> bind(const std::string& queueName);

private:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackPhysicalConnection>>;
    std::map<std::string, std::shared_ptr<LoopbackConnectionQueue>> spawnedConnections_;
};
}
}