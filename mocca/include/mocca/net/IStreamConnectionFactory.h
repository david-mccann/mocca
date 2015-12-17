#pragma once

#include "mocca/net/IStreamConnection.h"
#include "mocca/net/IStreamConnectionAcceptor.h"

namespace mocca {
namespace net {
class IStreamConnectionFactory {
public:
    virtual ~IStreamConnectionFactory() {}

    virtual std::unique_ptr<IStreamConnection> connect(const std::string& address) = 0;
    virtual std::unique_ptr<IStreamConnectionAcceptor> bind(const std::string& address) = 0;
};
}
}