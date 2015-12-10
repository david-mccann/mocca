#pragma once

#include <memory>
#include <string>

namespace mocca {
namespace net {

class IProtocolConnection;
class IProtocolConnectionAcceptor;

class IProtocolNetworkService {
public:
    virtual ~IProtocolNetworkService() {}

    virtual std::string protocol() const = 0;
    virtual std::unique_ptr<IProtocolConnection> connect(const std::string& address) = 0;
    virtual std::unique_ptr<IProtocolConnectionAcceptor> bind(const std::string& bindingString) = 0;
};
}
}