#pragma once

#include "mocca/net/PhysicalConnection.h"
#include "mocca/net/TCPConnection.h"
#include "mocca/net/TCPConnectionAcceptor.h"

namespace mocca {
namespace net {

class TCPNetworkService {
public:
    using ConnectionType = TCPConnection;
    using AcceptorType = TCPConnectionAcceptor;

    static std::string protocol();
    std::unique_ptr<TCPConnection> connect(const std::string& address);
    std::unique_ptr<TCPConnectionAcceptor> bind(const std::string& portString);
    std::string localIp() const;

private:
#ifdef WIN32
    std::string getLastWinsockError() const;
#endif
};
}
}