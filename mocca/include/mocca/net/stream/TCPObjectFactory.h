#pragma once

#include "mocca/net/stream/StreamObjectFactory.h"
#include "mocca/net/stream/TCPStream.h"
#include "mocca/net/stream/TCPStreamAcceptor.h"

namespace mocca {
namespace net {
class TCPObjectFactory : public StreamObjectFactory<TCPObjectFactory, TCPStream, TCPStreamAcceptor> {
    friend class StreamObjectFactory<TCPObjectFactory, TCPStream, TCPStreamAcceptor>;
private:
    std::unique_ptr<TCPStream> createStreamImpl(const std::string& args);
    std::unique_ptr<TCPStreamAcceptor> createAcceptorImpl(const std::string& args);
};
}
}
