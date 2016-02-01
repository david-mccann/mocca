/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include <string>

namespace mocca {
namespace net {

struct Endpoint {
    Endpoint() = default;
    Endpoint(const std::string& protocol, const std::string& machine, const std::string& port);
    Endpoint(const std::string& str);

    bool equals(const Endpoint& other) const;
    friend bool operator==(const Endpoint& lhs, const Endpoint& rhs);
    friend bool operator!=(const Endpoint& lhs, const Endpoint& rhs);

    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Endpoint& obj);
    std::string address() const;

    std::string protocol;
    std::string machine;
    std::string port;
};

struct TCPEndpoint : public Endpoint {
    TCPEndpoint() = default;
    TCPEndpoint(const std::string& ip, const std::string& port);
    TCPEndpoint(const std::string& str);
};

struct WSEndpoint : public Endpoint {
    WSEndpoint() = default;
    WSEndpoint(const std::string& ip, const std::string& port);
    WSEndpoint(const std::string& str);
};

bool isTCPLocalhost(const Endpoint& endpoint);
}
}
