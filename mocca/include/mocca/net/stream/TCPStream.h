#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/stream/Stream.h"
#include "mocca/net/stream/Sockets.h"
#include "mocca/net/stream/TCPNetworkAddress.h"

namespace mocca {
namespace net {

class TCPStream : public Stream<TCPStream> {
    friend class Stream<TCPStream>;

public:
    TCPStream(std::unique_ptr<IVDA::ConnectionSocket> socket);
    ~TCPStream();

private:
    std::string identifierImpl() const;
    void writeImpl(ByteArray message, std::chrono::milliseconds timeout);
    ByteArray readImpl(uint32_t maxSize, std::chrono::milliseconds timeout);

    static std::string createIdentifier();

private:
    std::unique_ptr<IVDA::ConnectionSocket> socket_;
    std::string identifier_;
};
}
}