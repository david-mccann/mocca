/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/Endpoint.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"

using namespace mocca::net;

Endpoint::Endpoint(const std::string& protocol, const std::string& machine, const std::string& port)
    : protocol(protocol)
    , machine(machine)
    , port(port) {}

Endpoint::Endpoint(const std::string& str) {
    auto split = mocca::splitString<std::string>(str, ':');
    if (split.size() != 3) {
        throw Error(
            mocca::formatString("Cannot initialize endpoint from string '%%' (does not match format <protocol>:<machine>:<port>)", str),
            __FILE__, __LINE__);
    }
    protocol = split[0];
    machine = split[1];
    port = split[2];
}

bool Endpoint::equals(const Endpoint& other) const {
    return protocol == other.protocol && machine == other.machine && port == other.port;
}

std::string Endpoint::toString() const {
    return protocol + ":" + machine + ":" + port;
}

std::string Endpoint::address() const {
    return machine + ":" + port;
}

namespace mocca {
namespace net {
bool operator==(const Endpoint& lhs, const Endpoint& rhs) {
    return lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const Endpoint& obj) {
    return os << obj.toString();
}

bool isTCPLocalhost(const Endpoint& endpoint) {
    return (endpoint.protocol.find("tcp") != std::string::npos) &&
           (endpoint.machine == "localhost" || endpoint.machine == "127.0.0.1");
}
}
}

TCPEndpoint::TCPEndpoint(const std::string& ip, const std::string& port)
    : Endpoint("tcp.prefixed", ip, port) {}

TCPEndpoint::TCPEndpoint(const std::string& str)
    : Endpoint("tcp.prefixed:" + str) {}


WSEndpoint::WSEndpoint(const std::string& ip, const std::string& port)
    : Endpoint("tcp.ws", ip, port) {}

WSEndpoint::WSEndpoint(const std::string& str)
    : Endpoint("tcp.ws:" + str) {}