#pragma once

#include "mocca/net/IProtocolNetworkService.h"
#include "mocca/net/MoccaConnectionAcceptor.h"
#include "mocca/net/MoccaConnection.h"

namespace mocca {
namespace net {

template <typename PhysicalServiceType>
class MoccaNetworkService : public IProtocolNetworkService {
public:
    static std::string protocolStatic() {
        return "mocca." + PhysicalServiceType::protocol();
    }

    std::string protocol() const override {
        return protocolStatic();
    }

    std::unique_ptr<IProtocolConnection> connect(const std::string& address) override {
        auto physicalConnection = physicalNetworkService_.connect(address);
        if (physicalConnection != nullptr) {
            return std::unique_ptr<IProtocolConnection>(new MoccaConnection(std::move(physicalConnection)));
        }
        return nullptr;
    }

    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& bindingString) override {
        auto physicalConnectionAcceptor = physicalNetworkService_.bind(bindingString);
        if (physicalConnectionAcceptor != nullptr) {
            return std::unique_ptr<IProtocolConnectionAcceptor>(new MoccaConnectionAcceptor(std::move(physicalConnectionAcceptor)));
        }
        return nullptr;
    }

private:
    PhysicalServiceType physicalNetworkService_;
};
}
}