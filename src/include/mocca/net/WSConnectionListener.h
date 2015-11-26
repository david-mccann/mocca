#pragma once

#include <thread>
#include <deque>
#include <atomic>

#include "mocca/net/IConnectionListener.h"
#include "mocca/net/WSConnection.h"

namespace mocca {
namespace net {

class WSConnectionListener : public IConnectionListener {
public:
	WSConnectionListener(const WSConnectionListener& other) = delete;
    std::unique_ptr<AbstractConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    friend class WSNetworkService;

	WSConnectionListener(int port);

    IVDA::TCPServer server_;
};
}
}