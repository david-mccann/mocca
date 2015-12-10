#pragma once

#include <deque>

#include "mocca/net/TCPConnection.h"

namespace mocca {
namespace net {

class TCPConnectionAcceptor {
public:
    using ConnectionType = TCPConnection;

    TCPConnectionAcceptor(const TCPConnectionAcceptor& other) = delete;
    std::unique_ptr<TCPConnection> getConnection(std::chrono::milliseconds timeout);

private:
    friend class TCPNetworkService;

    TCPConnectionAcceptor(int port);

    int port_;
    IVDA::TCPServer server_;
};
}
}