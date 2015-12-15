#pragma once

#include "mocca/net/IStreamConnectionFactory.h"

#include "mocca/net/stream/QueueConnectionAcceptor.h"

#include <map>

namespace mocca {
namespace net {
class QueueConnectionFactory : public IStreamConnectionFactory {
public:
    std::string protocol() const override;
    std::unique_ptr<IStreamConnection> connect(const std::string& queueName) override;
    std::unique_ptr<IStreamConnectionAcceptor> bind(const std::string& queueName) override;

private:
    std::map<std::string, std::shared_ptr<typename QueueConnectionAcceptor::LoopbackConnectionQueue>> spawnedConnections_;
};
}
}
