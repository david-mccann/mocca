#pragma once

#include "mocca/net/stream/Sockets.h"
#include "mocca/net/IStreamConnectionAcceptor.h"

namespace mocca {
namespace net {

class TCPConnectionAcceptor : public IStreamConnectionAcceptor {
public:
    TCPConnectionAcceptor(int port);
    std::string protocol() const override;
    Endpoint connectionEndpoint() const override;
    std::unique_ptr<IStreamConnection> accept(std::chrono::milliseconds timeout) override;

private:
    std::string getLastWinsockError() const;

private:
    int port_;
    IVDA::TCPServer server_;
};
}
}