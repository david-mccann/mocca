//#include "mocca/base/StringTools.h"
//#include "mocca/net/Error.h"
//#include "mocca/net/IProtocolConnection.h"
//#include "mocca/net/IProtocolConnectionAcceptor.h"
//#include "mocca/net/LoopbackNetworkService.h"
//#include "mocca/net/NetworkServiceLocator.h"
//#include "mocca/net/NetworkServiceLocator.h"
//#include "mocca/net/stream/TCPNetworkAddress.h"
//#include "mocca/net/stream/TCPNetworkService.h"
//
//using namespace mocca::net;
//
//std::vector<std::shared_ptr<IProtocolNetworkService>> NetworkServiceLocator::services_;
//
//std::shared_ptr<IProtocolNetworkService> NetworkServiceLocator::service(const std::string& protocol) {
//    for (auto& service : services_) {
//        if (service->protocol() == protocol) {
//            return service;
//        }
//    }
//    throw NetworkError(mocca::formatString("Network service for protocol '%%' is not available", protocol), __FILE__, __LINE__);
//}
//
//void NetworkServiceLocator::provideService(std::shared_ptr<IProtocolNetworkService> service) {
//    for (auto& s : services_) {
//        if (s->protocol() == service->protocol()) {
//            throw NetworkError(mocca::formatString("Network service for protocol '%%' already provided to locator", service->protocol()),
//                               __FILE__, __LINE__);
//        }
//    }
//    services_.push_back(service);
//}
//
//void mocca::net::NetworkServiceLocator::removeAll() {
//    services_.clear();
//}
//
//std::unique_ptr<IProtocolConnectionAcceptor> mocca::net::NetworkServiceLocator::bind(const Endpoint& endpoint) {
//    auto serv = service(endpoint.protocol());
//    return serv->bind(endpoint.address());
//}
//
//std::unique_ptr<IProtocolConnection> mocca::net::NetworkServiceLocator::connect(const Endpoint& endpoint) {
//    auto serv = service(endpoint.protocol());
//    return serv->connect(endpoint.address());
//}