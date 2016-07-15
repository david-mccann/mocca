#pragma once

#include "mocca/net/IMessageConnection.h"

#include <memory>

namespace mocca {
namespace net {

class AutoConnection {
public:
    AutoConnection(const Endpoint& endpoint);

    std::shared_ptr<const ConnectionID> connectionID() const;
    void send(Message message) const;
    Message receive() const;

private:
    Endpoint m_endpoint;
    mutable std::unique_ptr<IMessageConnection> m_connection;
};
}
}
