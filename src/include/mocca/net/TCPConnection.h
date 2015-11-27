#pragma once

#include "mocca/net/IPhysicalConnection.h"
#include "mocca/net/Sockets.h"
#include "mocca/net/TCPNetworkAddress.h"

#include <mutex>

namespace mocca {
namespace net {

class TCPConnection : public IPhysicalConnection {
public:
    TCPConnection(const TCPConnection& other) = delete;
    ~TCPConnection();

    std::string identifier() const override;
    void lock() override;
    void unlock() override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;
    ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout) const override;

private:
    static std::string createIdentifier();

private:
    friend class TCPNetworkService;
    friend class TCPConnectionAcceptor;

    TCPConnection(std::unique_ptr<IVDA::ConnectionSocket> socket);

    // TCPNetworkAddress networkAddress_;
    std::unique_ptr<IVDA::ConnectionSocket> socket_;
    std::string identifier_;
    std::mutex mutex_;
};
}
}