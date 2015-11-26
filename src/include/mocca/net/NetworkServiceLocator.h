#pragma once

#include "mocca/net/INetworkService.h"
#include "mocca/net/Endpoint.h"

#include <vector>

namespace mocca {
namespace net {

class AbstractConnection;
class IConnectionListener;

class NetworkServiceLocator {
public:
    static std::shared_ptr<INetworkService> service(const std::string& transport);
    static void provideService(std::shared_ptr<INetworkService> service);
    static void removeAll();

    static std::unique_ptr<IConnectionListener> bind(const Endpoint& endpoint);
    static std::unique_ptr<AbstractConnection> connect(const Endpoint& endpoint);

private:
    static std::vector<std::shared_ptr<INetworkService>> services_;
};
}
}