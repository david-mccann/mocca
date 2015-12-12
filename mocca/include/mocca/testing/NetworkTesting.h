#pragma once

#include <string>

namespace mocca {
namespace testing {
template <typename T> class TempProvideNetworkService {
public:
    TempProvideNetworkService() { net::NetworkServiceLocator::provideService(std::make_shared<T>()); }
    ~TempProvideNetworkService() { net::NetworkServiceLocator::removeAll(); }
};

std::string createAddress(const std::string& protocol, int index = 0);
std::string createBindingAddress(const std::string& protocol, int index = 0);

//template <typename T> net::Endpoint createConnectionEndpoint(int index = 0);
//template <typename T> net::Endpoint createBindingEndpoint(int index = 0);
//template <> net::Endpoint createConnectionEndpoint<net::TCPObjectFactory>(int index);
//template <> net::Endpoint createBindingEndpoint<net::TCPObjectFactory>(int index);
//template <> net::Endpoint createConnectionEndpoint<net::LoopbackPhysicalNetworkService>(int index);
//template <> net::Endpoint createBindingEndpoint<net::LoopbackPhysicalNetworkService>(int index);
//template <> net::Endpoint createConnectionEndpoint<net::LoopbackNetworkService>(int index);
//template <> net::Endpoint createBindingEndpoint<net::LoopbackNetworkService>(int index);
}
}