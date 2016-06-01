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

const std::string& Endpoint::autoPort() {
    static std::string port = "auto";
    return port;
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

bool operator!=(const Endpoint& lhs, const Endpoint& rhs) {
    return !lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const Endpoint& obj) {
    return os << obj.toString();
}

bool isTCPLocalhost(const Endpoint& endpoint) {
    return (endpoint.protocol.find("tcp") != std::string::npos) && (endpoint.machine == "localhost" || endpoint.machine == "127.0.0.1");
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