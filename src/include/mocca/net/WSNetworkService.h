#pragma once

#include "mocca/net/INetworkService.h"

namespace mocca {
namespace net {

class WSNetworkService : public INetworkService {
public:
    static std::string transportStatic() { return "ws"; }
    std::string transport() const override { return transportStatic(); }
    std::unique_ptr<AbstractConnection> connect(const std::string& connectionString) override;
    std::unique_ptr<IConnectionListener> bind(const std::string& portString) override;

};
}
}