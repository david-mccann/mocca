#pragma once

#include "mocca/net/IStreamConnectionFactory.h"

namespace mocca {
namespace net {
class TCPConnectionFactory : public IStreamConnectionFactory {
private:
    std::unique_ptr<IStreamConnection> connect(const std::string& address) override;
    std::unique_ptr<IStreamConnectionAcceptor> bind(const std::string& address) override;
};
}
}
