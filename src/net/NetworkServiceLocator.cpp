#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/Error.h"
#include "mocca/net/AbstractConnection.h"
#include "mocca/net/IConnectionListener.h"
#include "mocca/net/LoopbackNetworkService.h"
#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/TCPNetworkAddress.h"
#include "mocca/net/TCPNetworkService.h"

using namespace mocca::net;

std::vector<std::shared_ptr<INetworkService>> NetworkServiceLocator::services_;

std::shared_ptr<INetworkService> NetworkServiceLocator::service(const std::string& transport) {
    for (auto& service : services_) {
        if (service->transport() == transport) {
            return service;
        }
    }
    throw NetworkError("Network service for transport '" + transport + "' is not available", __FILE__, __LINE__);
}

void NetworkServiceLocator::provideService(std::shared_ptr<INetworkService> service) {
    for (auto& service : services_) {
        if (service->transport() == service->transport()) {
            throw NetworkError("Network service for transport '" + service->transport() +
                                   "' already provided to locator", __FILE__, __LINE__);
        }
    }
    services_.push_back(service);
}

void mocca::net::NetworkServiceLocator::removeAll() { services_.clear(); }

std::unique_ptr<IConnectionListener>
mocca::net::NetworkServiceLocator::bind(const Endpoint& endpoint) {
    auto serv = service(endpoint.transport());
    return serv->bind(endpoint.connectionString());
}

std::unique_ptr<AbstractConnection>
mocca::net::NetworkServiceLocator::connect(const Endpoint& endpoint) {
    auto serv = service(endpoint.transport());
    return serv->connect(endpoint.connectionString());
}