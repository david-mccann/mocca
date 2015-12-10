#pragma once

#include "mocca/net/PhysicalConnection.h"
#include "mocca/net/IPhysicalConnectionAcceptor.h"

namespace mocca {
namespace net {

class TCPNetworkService {
public:
    static std::string transportStatic();
    std::string transport() const;
    std::unique_ptr<IPhysicalConnection> connect(const std::string& connectionString);
    std::unique_ptr<IPhysicalConnectionAcceptor> bind(const std::string& portString);
    std::string localIp() const;
    
private:
#ifdef WIN32
    std::string getLastWinsockError() const;
#endif

};
}
}