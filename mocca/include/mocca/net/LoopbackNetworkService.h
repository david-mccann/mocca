#pragma once

#include "mocca/net/IProtocolNetworkService.h"
#include "mocca/net/LoopbackConnection.h"

#include <map>

namespace mocca {
template <typename T> class MessageQueue;
namespace net {

class LoopbackNetworkService : public IProtocolNetworkService {
public:
    static std::string protocolStatic();
    std::string protocol() const override;
    std::unique_ptr<IProtocolConnection> connect(const std::string& queueName) override;
    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& queueName) override;

private:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackConnection>>;
    std::map<std::string, std::shared_ptr<LoopbackConnectionQueue>> spawnedConnections_;
};
}
}