#pragma once

#include "mocca/net/stream/TCPStream.h"
#include "mocca/net/stream/TCPStreamAcceptor.h"

namespace mocca {
namespace net {

class TCPNetworkService {
public:
    using ConnectionType = TCPStream;
    using AcceptorType = TCPStreamAcceptor;

    static std::string protocol();
    std::unique_ptr<TCPStream> connect(const std::string& address);
    std::unique_ptr<TCPStreamAcceptor> bind(const std::string& portString);
    std::string localIp() const;

private:
#ifdef WIN32
    std::string getLastWinsockError() const;
#endif
};
}
}