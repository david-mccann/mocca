#pragma once

#include "mocca/net/IPhysicalConnection.h"
#include "mocca/net/Sockets.h"
#include "mocca/net/TCPNetworkAddress.h"

#include <string>
#include <mutex>

namespace mocca {
namespace net {

class TCPConnection : public IPhysicalConnection {
public:
    TCPConnection(const TCPConnection& other) = delete;
    ~TCPConnection();

    TCPNetworkAddress networkAddress() const;

    std::string identifier() const override;
    void send(ByteArray message) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    friend class TCPNetworkService;
    friend class TCPConnectionAcceptor;

    TCPConnection(const TCPNetworkAddress& networkAddress, std::unique_ptr<IVDA::ConnectionSocket> socket);

    TCPNetworkAddress networkAddress_;
    std::unique_ptr<IVDA::ConnectionSocket> socket_;
    mutable std::mutex sendMx_;
    mutable std::mutex receiveMx_;
};
}
}