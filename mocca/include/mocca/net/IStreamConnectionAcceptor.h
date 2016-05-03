/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnection.h"

#include <chrono>
#include <memory>

namespace mocca {
namespace net {

class IStreamConnectionAcceptor {
public:
    virtual ~IStreamConnectionAcceptor() {}

    virtual std::unique_ptr<IStreamConnection> accept(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
    virtual std::shared_ptr<const Endpoint> localEndpoint() const = 0;
};
}
}