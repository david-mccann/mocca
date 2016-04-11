/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/base/Nullable.h"
#include "mocca/net/IStreamConnection.h"
#include "mocca/net/stream/Sockets.h"

namespace mocca {
namespace net {

class TCPConnection : public IStreamConnection {
public:
    TCPConnection(std::unique_ptr<IVDB::ConnectionSocket> socket);
    ~TCPConnection();

    std::shared_ptr<const ConnectionID> connectionID() const override;
    bool isConnected() const override;
    uint32_t send(const uint8_t* data, uint32_t size, std::chrono::milliseconds timeout) const override;
    uint32_t receive(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout) const override;

private:
    std::unique_ptr<IVDB::ConnectionSocket> socket_;
    mutable std::shared_ptr<const ConnectionID> connectionID_;
};
}
}