#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/stream/IOStreamBase.h"
#include "mocca/net/stream/Sockets.h"
#include "mocca/net/stream/TCPNetworkAddress.h"

namespace mocca {
namespace net {

class TCPConnection : public IOStreamBase<TCPConnection> {
    friend class IOStreamBase<TCPConnection>;
    friend class TCPNetworkService;
    friend class TCPConnectionAcceptor;

public:
    TCPConnection(const TCPConnection& other) = delete;
    ~TCPConnection();

private:
    std::string identifierImpl() const;
    void writeImpl(ByteArray message, std::chrono::milliseconds timeout);
    ByteArray readImpl(uint32_t maxSize, std::chrono::milliseconds timeout);

    static std::string createIdentifier();

private:
    TCPConnection(std::unique_ptr<IVDA::ConnectionSocket> socket);

    std::unique_ptr<IVDA::ConnectionSocket> socket_;
    std::string identifier_;
};
}
}