/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnection.h"
#include "mocca/net/IStreamConnectionAcceptor.h"

namespace mocca {
namespace net {
class IStreamConnectionFactory {
public:
    virtual ~IStreamConnectionFactory() {}

    virtual std::unique_ptr<IStreamConnection> connect(const std::string& address) = 0;
    virtual std::unique_ptr<IStreamConnectionAcceptor> bind(const std::string& machine, const std::string& port) = 0;
};
}
}