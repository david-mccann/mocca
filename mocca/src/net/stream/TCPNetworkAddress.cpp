#include "mocca/net/stream/TCPNetworkAddress.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"

namespace mocca {
namespace net {

TCPNetworkAddress::TCPNetworkAddress(const std::string& ip, int port) {
    checkIp(ip);
    checkPort(port);
    ip_ = ip;
    port_ = port;
}

mocca::net::TCPNetworkAddress::TCPNetworkAddress(const std::string& address) {
    auto parts = mocca::splitString<std::string>(address, ':');
    if (parts.size() != 2) {
        throw Error("TCP connection string is malformed, must have the format 'ip:port'", __FILE__, __LINE__);
    }
    auto ip = parts.front();
    auto port = parsePort(parts.back());
    checkIp(ip);
    checkPort(port);
    ip_ = ip;
    port_ = port;
}

void mocca::net::TCPNetworkAddress::checkIp(const std::string& ip) {
    if (ip.empty()) {
        throw Error("Empty IP is invalid", __FILE__, __LINE__);
    }
}

void mocca::net::TCPNetworkAddress::checkPort(int port) {
    if (port < 1 || port > 65535) {
        throw Error("Port number " + std::to_string(port) + " is out of range", __FILE__, __LINE__);
    }
}

int mocca::net::TCPNetworkAddress::parsePort(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::exception& err) {
        throw Error("Error parsing port number '" + str + "': " + err.what(), __FILE__, __LINE__);
    }
}

std::string TCPNetworkAddress::ip() const {
    return ip_;
}

int TCPNetworkAddress::port() const {
    return port_;
}

std::string mocca::net::TCPNetworkAddress::address() const {
    return ip_ + ":" + std::to_string(port_);
}

std::string mocca::net::TCPNetworkAddress::bindingString() const {
    return std::to_string(port_);
}

std::string mocca::net::TCPNetworkAddress::toString() const {
    return ip_ + ":" + std::to_string(port_);
}

std::ostream& operator<<(std::ostream& os, const TCPNetworkAddress& obj) {
    return os << obj.toString();
}
}
}