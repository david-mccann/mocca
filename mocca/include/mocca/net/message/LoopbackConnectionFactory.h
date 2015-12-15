#pragma once

#include "mocca/net/IMessageConnectionFactory.h"

#include "mocca/net/message/LoopbackConnectionAcceptor.h"

#include <map>

namespace mocca {
namespace net {
class LoopbackConnectionFactory : public IMessageConnectionFactory {
public:
    std::string protocol() const override;
    std::unique_ptr<IMessageConnection> connect(const std::string& queueName) override;
    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& queueName) override;

private:
    std::map<std::string, std::shared_ptr<typename LoopbackConnectionAcceptor::LoopbackConnectionQueue>> spawnedConnections_;
};
}
}
