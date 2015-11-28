#pragma once

#include "mocca/net/IProtocolNetworkService.h"
#include "mocca/net/Endpoint.h"

#include <vector>

namespace mocca {
namespace net {

class IProtocolConnection;
class IProtocolConnectionAcceptor;

class NetworkServiceLocator {
public:
    static std::shared_ptr<IProtocolNetworkService> service(const std::string& protocol, const std::string& transport);
    static void provideService(std::shared_ptr<IProtocolNetworkService> service);
    static void removeAll();

    static std::unique_ptr<IProtocolConnectionAcceptor> bind(const Endpoint& endpoint);
    static std::unique_ptr<IProtocolConnection> connect(const Endpoint& endpoint);

private:
    static std::vector<std::shared_ptr<IProtocolNetworkService>> services_;
};
}
}