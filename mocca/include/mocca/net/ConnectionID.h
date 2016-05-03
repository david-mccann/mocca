/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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
