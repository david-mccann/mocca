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

#include "mocca/net/Endpoint.h"

namespace mocca {
namespace net {
struct ConnectionID {
    ConnectionID() = default;
    ConnectionID(const Endpoint& local, const Endpoint& peer)
        : localEndpoint(local)
        , peerEndpoint(peer) {}
    Endpoint localEndpoint;
    Endpoint peerEndpoint;

    std::string toString() const { return localEndpoint.toString() + "/" + peerEndpoint.toString(); }
    friend std::ostream& operator<<(std::ostream& os, const ConnectionID& obj) { return os << obj.toString(); }

    bool equals(const ConnectionID& other) const { return localEndpoint == other.localEndpoint && peerEndpoint == other.peerEndpoint; }
    friend bool operator==(const ConnectionID& lhs, const ConnectionID& rhs) { return lhs.equals(rhs); }
};
}
}
