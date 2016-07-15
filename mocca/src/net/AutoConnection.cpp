#include "mocca/net/AutoConnection.h"

#include "mocca/net/ConnectionFactorySelector.h"

using namespace mocca::net;

AutoConnection::AutoConnection(const Endpoint& endpoint)
    : m_endpoint(endpoint)
    , m_connection(nullptr) {}

std::shared_ptr<const ConnectionID> AutoConnection::connectionID() const {
    if (m_connection == nullptr || !m_connection->isConnected()) {
        m_connection = ConnectionFactorySelector::connect(m_endpoint);
    }
    return m_connection->connectionID();
}

void mocca::net::AutoConnection::send(Message message) const {
    if (m_connection == nullptr || !m_connection->isConnected()) {
        m_connection = ConnectionFactorySelector::connect(m_endpoint);
    }
    m_connection->send(std::move(message));
}

mocca::net::Message mocca::net::AutoConnection::receive() const {
    if (m_connection == nullptr || !m_connection->isConnected()) {
        m_connection = ConnectionFactorySelector::connect(m_endpoint);
    }
    return m_connection->receive();
}
