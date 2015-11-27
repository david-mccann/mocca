#pragma once

#include "mocca/net/IPhysicalNetworkService.h"

namespace mocca {
namespace net {

class WSNetworkService : public IPhysicalNetworkService {
public:
    static std::string transportStatic() { return "ws"; }
    std::string transport() const override { return transportStatic(); }
    std::unique_ptr<IPhysicalConnection> connect(const std::string& connectionString) override;
    std::unique_ptr<IPhysicalConnectionAcceptor> bind(const std::string& portString) override;

};
}
}