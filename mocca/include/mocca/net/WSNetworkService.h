#pragma once

#include "mocca/net/IProtocolNetworkService.h"

namespace mocca {
namespace net {

template <typename PhysicalServiceType>
class WSNetworkService : public IProtocolNetworkService {
public:
    static std::string protocolStatic() {
        return "ws." + PhysicalServiceType::protocol();
    }

    std::string protocol() const override {
        return protocolStatic();
    }

    std::unique_ptr<IProtocolConnection> connect(const std::string& address) override {
        throw Error("WebSocket client connections not supported", __FILE__, __LINE__);
    }
    
    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& bindingString) override {
        auto physicalConnectionAcceptor = physicalNetworkService_.bind(bindingString);
        if (physicalConnectionAcceptor != nullptr) {
            return std::unique_ptr<IProtocolConnectionAcceptor>(new WSConnectionAcceptor(std::move(physicalConnectionAcceptor)));
        }
        return nullptr;
    }

    PhysicalServiceType& physicalService() {
        return physicalNetworkService_;
    }

private:
    PhysicalServiceType physicalNetworkService_;
};
}
}