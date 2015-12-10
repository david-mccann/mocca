#pragma once

#include "mocca/net/LoopbackConnection.h"
#include "mocca/net/IProtocolConnectionAcceptor.h"

#include <map>


namespace mocca {

template<typename T> class MessageQueue;

namespace net {

class LoopbackNetworkService {
public:
    static std::string transportStatic();
    std::string transport() const;
    static std::string protocolStatic();
    std::string protocol() const;
    std::unique_ptr<IProtocolConnection> connect(const std::string& queueName);
    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& queueName);

private:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackConnection>>;
    std::map<std::string, std::shared_ptr<LoopbackConnectionQueue>> spawnedConnections_;
};
}
}