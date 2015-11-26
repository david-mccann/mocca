#pragma once

#include "mocca/net/INetworkService.h"

namespace mocca {
namespace net {

class TCPNetworkService : public INetworkService {
public:
    static std::string transportStatic() { return "tcp"; }
    std::string transport() const override { return transportStatic(); }
    std::unique_ptr<AbstractConnection> connect(const std::string& connectionString) override;
    std::unique_ptr<IConnectionListener> bind(const std::string& portString) override;
    std::string localIp() const;
    
private:
#ifdef WIN32
    std::string getLastWinsockError() const;
#endif

};
}
}