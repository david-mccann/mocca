#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/IStreamConnection.h"
#include "mocca/net/stream/Sockets.h"
#include "mocca/net/stream/TCPNetworkAddress.h"

namespace mocca {
namespace net {

class TCPConnection : public IStreamConnection {
public:
    TCPConnection(std::unique_ptr<IVDA::ConnectionSocket> socket);
    ~TCPConnection();

    std::string identifier() const override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;
    ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout) const override;

private:
    static std::string createIdentifier();

private:
    std::unique_ptr<IVDA::ConnectionSocket> socket_;
    std::string identifier_;
};
}
}