#pragma once

#include <thread>
#include <deque>
#include <atomic>

#include "mocca/net/IProtocolConnectionAcceptor.h"
#include "mocca/net/WSConnection.h"

namespace mocca {
namespace net {

class WSConnectionListener : public IProtocolConnectionAcceptor {
public:
	WSConnectionListener(const WSConnectionListener& other) = delete;
    std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    friend class WSNetworkService;

	WSConnectionListener(int port);

    IVDA::TCPServer server_;
};
}
}