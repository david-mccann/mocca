//#pragma once
//
//#include "mocca/net/IProtocolConnectionAcceptor.h"
//#include "mocca/net/LoopbackNetworkService.h"
//#include "mocca/net/NetworkServiceLocator.h"
//#include "mocca/net/stream/TCPNetworkService.h"
//#include "mocca/testing/LoopbackPhysicalNetworkService.h"
//
//namespace mocca {
//namespace testing {
//template <typename T> class TempProvideNetworkService {
//public:
//    TempProvideNetworkService() { net::NetworkServiceLocator::provideService(std::make_shared<T>()); }
//    ~TempProvideNetworkService() { net::NetworkServiceLocator::removeAll(); }
//};
//
//template <typename T> std::string createAddress(int index = 0);
//template <typename T> std::string createBindingAddress(int index = 0);
//template <> std::string createAddress<net::TCPNetworkService>(int index);
//template <> std::string createBindingAddress<net::TCPNetworkService>(int index);
//template <> std::string createAddress<net::LoopbackPhysicalNetworkService>(int index);
//template <> std::string createBindingAddress<net::LoopbackPhysicalNetworkService>(int index);
//template <> std::string createAddress<net::LoopbackNetworkService>(int index);
//template <> std::string createBindingAddress<net::LoopbackNetworkService>(int index);
//
//template <typename T> net::Endpoint createConnectionEndpoint(int index = 0);
//template <typename T> net::Endpoint createBindingEndpoint(int index = 0);
//template <> net::Endpoint createConnectionEndpoint<net::TCPNetworkService>(int index);
//template <> net::Endpoint createBindingEndpoint<net::TCPNetworkService>(int index);
//template <> net::Endpoint createConnectionEndpoint<net::LoopbackPhysicalNetworkService>(int index);
//template <> net::Endpoint createBindingEndpoint<net::LoopbackPhysicalNetworkService>(int index);
//template <> net::Endpoint createConnectionEndpoint<net::LoopbackNetworkService>(int index);
//template <> net::Endpoint createBindingEndpoint<net::LoopbackNetworkService>(int index);
//}
//}