#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnectionFactory.h"
#include "mocca/net/IStreamConnectionFactory.h"

#include <vector>
#include <memory>

namespace mocca {
namespace net {
class ConnectionFactorySelector {
public:
    static IMessageConnectionFactory& messageConnectionFactory(const std::string& protocol);
    static IStreamConnectionFactory& streamConnectionFactory(const std::string& protocol);
    
    static void addMessageConnectionFactory(std::unique_ptr<IMessageConnectionFactory> factory, const std::string& protocol);
    static void addStreamConnectionFactory(std::unique_ptr<IStreamConnectionFactory> factory, const std::string& protocol);
    static void addDefaultFactories();

    static void removeAll();

    static const std::string& tcp();
    static const std::string& queue();

    static const std::string& loopback();
    static const std::string& tcpPrefixed();
    static const std::string& tcpWebSocket();
    static const std::string& queuePrefixed();
    static const std::string& queueWebSocket();

    static std::unique_ptr<IMessageConnectionAcceptor> bind(const Endpoint& endpoint);
    static std::unique_ptr<IMessageConnection> connect(const Endpoint& endpoint);

private:
    static std::vector<std::pair<std::string, std::unique_ptr<IMessageConnectionFactory>>> messageConnectionFactories_;
    static std::vector<std::pair<std::string, std::unique_ptr<IStreamConnectionFactory>>> streamConnectionFactories_;
};
}
}