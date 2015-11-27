#pragma once

#include "mocca/net/IProtocolNetworkService.h"

namespace mocca {
namespace net {

class TCPNetworkService : public IProtocolNetworkService {
public:
    static std::string transportStatic() { return "tcp"; }
    std::string transport() const override { return transportStatic(); }
    std::unique_ptr<IProtocolConnection> connect(const std::string& connectionString) override;
    std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& portString) override;
    std::string localIp() const;
    
private:
#ifdef WIN32
    std::string getLastWinsockError() const;
#endif

};
}
}