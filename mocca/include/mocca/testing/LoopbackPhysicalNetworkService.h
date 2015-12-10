#pragma once

#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"

#include <map>

namespace mocca {
template <typename T> class MessageQueue;
namespace net {

class LoopbackPhysicalNetworkService {
public:
    using ConnectionType = LoopbackPhysicalConnection;
    using AcceptorType = LoopbackPhysicalConnectionAcceptor;

    static std::string protocol();
    std::unique_ptr<LoopbackPhysicalConnection> connect(const std::string& queueName);
    std::unique_ptr<LoopbackPhysicalConnectionAcceptor> bind(const std::string& queueName);

private:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackPhysicalConnection>>;
    std::map<std::string, std::shared_ptr<LoopbackConnectionQueue>> spawnedConnections_;
};
}
}