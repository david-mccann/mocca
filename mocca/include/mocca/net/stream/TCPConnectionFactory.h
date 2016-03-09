/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnectionFactory.h"

namespace mocca {
namespace net {
class TCPConnectionFactory : public IStreamConnectionFactory {
private:
    std::unique_ptr<IStreamConnection> connect(const std::string& address) override;
    std::unique_ptr<IStreamConnectionAcceptor> bind(const std::string& machine, const std::string& port) override;
};
}
}
