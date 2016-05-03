/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IMessageConnection.h"
#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class FramingConnection : public IMessageConnection {
public:
    FramingConnection(std::unique_ptr<IStreamConnection> stream, std::unique_ptr<FramingStrategy> framingStrategy);

    std::shared_ptr<const ConnectionID> connectionID() const override;
    bool isConnected() const override;
    void send(Message message) const override;
    Message receive() const override;

private:
    std::unique_ptr<IStreamConnection> stream_;
    std::unique_ptr<FramingStrategy> framingStrategy_;
    std::shared_ptr<const ConnectionID> connectionID_;
};
}
}
