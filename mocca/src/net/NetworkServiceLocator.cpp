#include "mocca/base/StringTools.h"
#include "mocca/net/Error.h"
#include "mocca/net/IProtocolConnection.h"
#include "mocca/net/IProtocolConnectionAcceptor.h"
#include "mocca/net/LoopbackNetworkService.h"
#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/TCPNetworkAddress.h"
#include "mocca/net/TCPNetworkService.h"

using namespace mocca::net;

std::vector<std::shared_ptr<IProtocolNetworkService>> NetworkServiceLocator::services_;

std::shared_ptr<IProtocolNetworkService> NetworkServiceLocator::service(const std::string& protocol, const std::string& transport) {
    for (auto& service : services_) {
        if (service->protocol() == protocol && service->transport() == transport) {
            return service;
        }
    }
    throw NetworkError("Network service for transport '" + transport + "' is not available", __FILE__, __LINE__);
}

void NetworkServiceLocator::provideService(std::shared_ptr<IProtocolNetworkService> service) {
    for (auto& s : services_) {
        if (s->protocol() == service->protocol() && s->transport() == service->transport()) {
            throw NetworkError(mocca::formatString("Network service for protocol '%%' and transport '%%' already provided to locator",
                                                   service->protocol(), service->transport()),
                               __FILE__, __LINE__);
        }
    }
    services_.push_back(service);
}

void mocca::net::NetworkServiceLocator::removeAll() {
    services_.clear();
}

std::unique_ptr<IProtocolConnectionAcceptor> mocca::net::NetworkServiceLocator::bind(const Endpoint& endpoint) {
    auto serv = service(endpoint.protocol(), endpoint.transport());
    return serv->bind(endpoint.connectionString());
}

std::unique_ptr<IProtocolConnection> mocca::net::NetworkServiceLocator::connect(const Endpoint& endpoint) {
    auto serv = service(endpoint.protocol(), endpoint.transport());
    return serv->connect(endpoint.connectionString());
}