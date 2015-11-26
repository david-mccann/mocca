#pragma once

#include <thread>
#include <deque>
#include <atomic>

#include "mocca/net/IConnectionListener.h"
#include "mocca/net/TCPConnection.h"

namespace mocca {
namespace net {

class TCPConnectionListener : public IConnectionListener {
public:
    TCPConnectionListener(const TCPConnectionListener& other) = delete;
    std::unique_ptr<AbstractConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    friend class TCPNetworkService;

    TCPConnectionListener(int port);

    IVDA::TCPServer server_;
};
}
}