/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/Message.h"
#include "mocca/net/ConnectionID.h"

#include <chrono>
#include <string>

namespace mocca {
namespace net {

class IMessageConnection {
public:
    virtual ~IMessageConnection() {}

    virtual std::shared_ptr<const ConnectionID> connectionID() const = 0;
    virtual bool isConnected() const = 0;
    virtual void send(Message message) const = 0;
    virtual Message receive(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
};
}
}