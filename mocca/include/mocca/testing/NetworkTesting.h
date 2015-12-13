#pragma once

#include "mocca/net/Endpoint.h"

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

net::Endpoint createConnectionEndpoint(const std::string& protocol, int index = 0);
net::Endpoint createBindingEndpoint(const std::string& protocol, int index = 0);
}
}