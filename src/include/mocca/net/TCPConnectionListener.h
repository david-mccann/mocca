#pragma once

#include <thread>
#include <deque>
#include <atomic>

#include "mocca/net/IPhysicalConnectionAcceptor.h"
#include "mocca/net/TCPConnection.h"

namespace mocca {
namespace net {

class TCPConnectionListener : public IPhysicalConnectionAcceptor {
public:
    TCPConnectionListener(const TCPConnectionListener& other) = delete;
    std::unique_ptr<IPhysicalConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    friend class TCPNetworkService;

    TCPConnectionListener(int port);

    IVDA::TCPServer server_;
};
}
}