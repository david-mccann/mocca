#pragma once

#include <memory>
#include <string>

namespace mocca {   
namespace net {

class IPhysicalConnection;
class IPhysicalConnectionAcceptor;

class IPhysicalNetworkService {
public:
    virtual ~IPhysicalNetworkService() {}

    virtual std::string transport() const = 0;
    virtual std::unique_ptr<IPhysicalConnection> connect(const std::string& connectionString) = 0;
    virtual std::unique_ptr<IPhysicalConnectionAcceptor> bind(const std::string& bindingString) = 0;
};
}
}