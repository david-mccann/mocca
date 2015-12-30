#pragma once

#include "mocca/net/IMessageConnectionFactory.h"
#include "mocca/net/message/LoopbackConnectionSpawner.h"

#include <vector>

namespace mocca {
namespace net {
class LoopbackConnectionFactory : public IMessageConnectionFactory {
public:
    std::unique_ptr<IMessageConnection> connect(const std::string& name) override;
    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& name) override;

private:
    std::shared_ptr<LoopbackConnectionSpawner> getSpawner(const std::string& name);

private:
    std::vector<std::shared_ptr<LoopbackConnectionSpawner>> spawners_;
};
}
}
