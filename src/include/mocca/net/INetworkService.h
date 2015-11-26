#pragma once

#include <memory>
#include <string>

namespace mocca {   
namespace net {

class AbstractConnection;
class IConnectionListener;

class INetworkService {
public:
    virtual ~INetworkService() {}

    virtual std::string transport() const = 0;
    virtual std::unique_ptr<AbstractConnection> connect(const std::string& connectionString) = 0;
    virtual std::unique_ptr<IConnectionListener> bind(const std::string& bindingString) = 0;
};
}
}