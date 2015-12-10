#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/Sockets.h"
#include "mocca/net/TCPNetworkAddress.h"

#include <mutex>

namespace mocca {
namespace net {

class TCPConnection {
public:
    TCPConnection(const TCPConnection& other) = delete;
    ~TCPConnection();

    std::string identifier() const;
    void lock();
    void unlock();
    void send(ByteArray message, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const;
    ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const;

private:
    static std::string createIdentifier();

private:
    friend class TCPNetworkService;
    friend class TCPConnectionAcceptor;

    TCPConnection(std::unique_ptr<IVDA::ConnectionSocket> socket);

    std::unique_ptr<IVDA::ConnectionSocket> socket_;
    std::string identifier_;
    std::mutex mutex_;
};
}
}