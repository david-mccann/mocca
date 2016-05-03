/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/Endpoint.h"

#include <string>


namespace mocca {
namespace testing {
std::string createAddress(const std::string& protocol, int index = 0);
std::string createBindingMachine(const std::string& protocol);
std::string createBindingPort(const std::string& protocol, int index = 0);

net::Endpoint createConnectionEndpoint(const std::string& protocol, int index = 0);
net::Endpoint createBindingEndpoint(const std::string& protocol, int index = 0);
}
}