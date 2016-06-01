/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

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
    if (port < 0 || port > 65535) {
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