/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnection.h"
#include "mocca/net/stream/Sockets.h"
#include "mocca/base/Nullable.h"

namespace mocca {
namespace net {

class TCPConnection : public IStreamConnection {
public:
    TCPConnection(std::unique_ptr<IVDA::ConnectionSocket> socket);
    ~TCPConnection();

    std::shared_ptr<const ConnectionID> connectionID() const override;
    bool isConnected() const override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;

private:
    ByteArray readFromStream(uint32_t maxSize, std::chrono::milliseconds timeout) const override;

private:
    std::unique_ptr<IVDA::ConnectionSocket> socket_;
    mutable std::shared_ptr<const ConnectionID> connectionID_;
};
}
}