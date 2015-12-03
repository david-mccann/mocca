#pragma once

#include "mocca/net/IProtocolNetworkService.h"
#include "mocca/net/IPhysicalNetworkService.h"


namespace mocca {
namespace net {

class WSNetworkService : public IProtocolNetworkService {
public:
    WSNetworkService(std::unique_ptr<IPhysicalNetworkService> physicalNetworkService);

    static std::string protocolStatic();
    std::string protocol() const override;
    std::string transport() const override;
    std::unique_ptr<IProtocolConnection> connect(const std::string& connectionString) override;
    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& bindingString) override;

    IPhysicalNetworkService& physicalService();

private:
    std::unique_ptr<IPhysicalNetworkService> physicalNetworkService_;
};
}
}