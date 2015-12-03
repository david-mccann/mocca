#pragma once

#include "mocca/net/IProtocolNetworkService.h"
#include "mocca/net/IPhysicalNetworkService.h"

namespace mocca {
namespace net {
class MoccaNetworkService : public IProtocolNetworkService {
public:
    MoccaNetworkService(std::unique_ptr<IPhysicalNetworkService> physicalNetworkService);

    std::string transport() const override;
    static std::string protocolStatic();
    std::string protocol() const override;
    std::unique_ptr<IProtocolConnection> connect(const std::string& connectionString) override;
    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& bindingString) override;

private:
    std::unique_ptr<IPhysicalNetworkService> physicalNetworkService_;
};
}
}