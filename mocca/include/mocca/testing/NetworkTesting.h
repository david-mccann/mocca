#pragma once

#include "mocca/net/Endpoint.h"

#include <string>


namespace mocca {
namespace testing {
std::string createAddress(const std::string& protocol, int index = 0);
std::string createBindingAddress(const std::string& protocol, int index = 0);

net::Endpoint createConnectionEndpoint(const std::string& protocol, int index = 0);
net::Endpoint createBindingEndpoint(const std::string& protocol, int index = 0);
}
}