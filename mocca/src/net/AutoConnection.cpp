#include "mocca/net/AutoConnection.h"

using namespace mocca::net;

AutoConnection::AutoConnection(const Endpoint& endpoint)
    : m_endpoint(endpoint)
    , m_connection(nullptr) {}

std::shared_ptr<const ConnectionID> AutoConnection::connectionID() const {
    return retry([&] {
        if (m_connection == nullptr || !m_connection->isConnected()) {
            m_connection = ConnectionFactorySelector::connect(m_endpoint);
        }
        return m_connection->connectionID();
    });
}

void AutoConnection::send(Message message) const {
    retry([&] {
        if (m_connection == nullptr || !m_connection->isConnected()) {
            m_connection = ConnectionFactorySelector::connect(m_endpoint);
        }
        m_connection->send(message);
        return 0;
    });
}

Message AutoConnection::receive() const {
    return retry([&] {
        if (m_connection == nullptr || !m_connection->isConnected()) {
            m_connection = ConnectionFactorySelector::connect(m_endpoint);
        }
        return m_connection->receive();
    });
}