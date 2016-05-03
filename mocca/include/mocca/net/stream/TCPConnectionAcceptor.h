/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnectionAcceptor.h"
#include "mocca/net/stream/Sockets.h"

namespace mocca {
namespace net {

class TCPConnectionAcceptor : public IStreamConnectionAcceptor {
public:
    TCPConnectionAcceptor(int port);

    std::unique_ptr<IStreamConnection> accept(std::chrono::milliseconds timeout) override;
    virtual std::shared_ptr<const Endpoint> localEndpoint() const override;
    
private:
    IVDB::TCPServer server_;
    std::shared_ptr<const Endpoint> endpoint_;
};
}
}