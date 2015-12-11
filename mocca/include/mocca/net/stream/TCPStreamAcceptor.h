#pragma once

#include <deque>

#include "mocca/net/stream/TCPStream.h"
#include "mocca/net/stream/StreamAcceptor.h"

namespace mocca {
namespace net {

class TCPStreamAcceptor : public StreamAcceptor<TCPStreamAcceptor, TCPStream> {
public:
    friend class StreamAcceptor<TCPStreamAcceptor, TCPStream>;
    TCPStreamAcceptor(int port);

private:
    std::unique_ptr<TCPStream> acceptImpl(std::chrono::milliseconds timeout);

private:
    int port_;
    IVDA::TCPServer server_;
};
}
}