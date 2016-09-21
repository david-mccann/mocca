#pragma once

#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/NetworkError.h"

#include <chrono>
#include <memory>
#include <thread>

namespace mocca {
namespace net {

class AutoConnection {
public:
    AutoConnection(const Endpoint& endpoint);

    std::shared_ptr<const ConnectionID> connectionID() const;
    void send(Message message) const;
    Message receive() const;

private:
    template <typename T> typename std::result_of<T()>::type retry(const T& fun) const {
        const int maxRetries = 10;
        auto waitTime = std::chrono::seconds(10);
        int retries = 0;
        while (true) {
            try {
                return fun();
            } catch (const mocca::net::NetworkError&) {
                ++retries;
                LWARNING("Failed to connect to " << m_endpoint << " (retry " << retries << ")");
                if (retries == maxRetries) {
                    throw;
                }
                std::this_thread::sleep_for(waitTime);
            }
        }
    }

private:
    Endpoint m_endpoint;
    mutable std::unique_ptr<IMessageConnection> m_connection;
};
}
}
