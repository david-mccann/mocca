/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IMessageConnection.h"

#include <chrono>
#include <memory>

namespace mocca {
namespace net {

class IMessageConnectionAcceptor {
public:
    virtual ~IMessageConnectionAcceptor() {}

    virtual std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
};
}
}