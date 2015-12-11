#pragma once

#include <deque>

#include "mocca/net/stream/TCPConnection.h"
#include "mocca/net/stream/StreamAcceptorBase.h"

namespace mocca {
namespace net {

class TCPConnectionAcceptor : public StreamAcceptorBase<TCPConnectionAcceptor, TCPConnection> {
public:
    friend class StreamAcceptorBase<TCPConnectionAcceptor, TCPConnection>;
    TCPConnectionAcceptor(const std::string& port);

private:
    std::unique_ptr<TCPConnection> acceptImpl(std::chrono::milliseconds timeout);

private:
    int port_;
    IVDA::TCPServer server_;
};
}
}