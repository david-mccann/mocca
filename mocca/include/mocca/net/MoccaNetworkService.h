#pragma once

#include "mocca/net/IProtocolNetworkService.h"
#include "mocca/net/MoccaConnection.h"
#include "mocca/net/MoccaConnectionAcceptor.h"

namespace mocca {
namespace net {

template <typename PhysicalServiceType> class MoccaNetworkService : public IProtocolNetworkService {
public:
    static std::string protocolStatic();
    std::string protocol() const override;

    std::unique_ptr<IProtocolConnection> connect(const std::string& address) override;
    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& bindingString) override;

private:
    PhysicalServiceType physicalNetworkService_;
};
}
}