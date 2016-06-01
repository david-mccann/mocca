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

#pragma once

#include <string>

namespace mocca {
namespace net {

struct Endpoint {
    Endpoint() = default;
    explicit Endpoint(const std::string& protocol, const std::string& machine, const std::string& port);
    explicit Endpoint(const std::string& str);

    static const std::string& autoPort();

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
