#pragma once

#include <thread>
#include <deque>
#include <atomic>

#include "mocca/net/IProtocolConnectionAcceptor.h"
#include "mocca/net/TCPConnection.h"

namespace mocca {
namespace net {

class TCPConnectionAcceptor : public IProtocolConnectionAcceptor {
public:
    TCPConnectionAcceptor(const TCPConnectionAcceptor& other) = delete;
    std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    friend class TCPNetworkService;

    TCPConnectionAcceptor(int port);

    IVDA::TCPServer server_;
};
}
}