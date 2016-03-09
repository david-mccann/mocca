/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

namespace mocca {
namespace net {
class IMessageConnectionFactory {
public:
    virtual ~IMessageConnectionFactory() {}

    virtual std::unique_ptr<IMessageConnection> connect(const std::string& address) = 0;
    virtual std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& machine, const std::string& port) = 0;
};
}
}