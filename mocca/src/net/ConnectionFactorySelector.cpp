/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/ConnectionFactorySelector.h"

#include "mocca/base/Error.h"
#include "mocca/base/Memory.h"
#include "mocca/base/StringTools.h"
#include "mocca/net/framing/SizePrefixedProtocol.h"
#include "mocca/net/framing/WebSocketProtocol.h"
#include "mocca/net/message/FramingConnectionFactory.h"
#include "mocca/net/message/LoopbackConnectionFactory.h"
#include "mocca/net/stream/QueueConnectionFactory.h"
#include "mocca/net/stream/TCPConnectionFactory.h"

using namespace mocca::net;

std::vector<std::pair<std::string, std::unique_ptr<IMessageConnectionFactory>>> ConnectionFactorySelector::messageConnectionFactories_;
std::vector<std::pair<std::string, std::unique_ptr<IStreamConnectionFactory>>> ConnectionFactorySelector::streamConnectionFactories_;

IMessageConnectionFactory& ConnectionFactorySelector::messageConnectionFactory(const std::string& protocol) {
    for (auto& factory : messageConnectionFactories_) {
        if (factory.first == protocol) {
            return *factory.second;
        }
    }
    throw Error(mocca::formatString("Factory for protocol '%%' is not available", protocol), __FILE__, __LINE__);
}

IStreamConnectionFactory& ConnectionFactorySelector::streamConnectionFactory(const std::string& protocol) {
    for (auto& factory : streamConnectionFactories_) {
        if (factory.first == protocol) {
            return *factory.second;
        }
    }
    throw Error(mocca::formatString("Factory for protocol '%%' is not available", protocol), __FILE__, __LINE__);
}

void ConnectionFactorySelector::addMessageConnectionFactory(std::unique_ptr<IMessageConnectionFactory> factory,
                                                            const std::string& protocol) {
    for (auto& fac : messageConnectionFactories_) {
        if (fac.first == protocol) {
            throw Error(mocca::formatString("Factory for protocol '%%' already provided to selector", fac.first), __FILE__, __LINE__);
        }
    }
    messageConnectionFactories_.push_back({protocol, std::move(factory)});
}

void ConnectionFactorySelector::addStreamConnectionFactory(std::unique_ptr<IStreamConnectionFactory> factory, const std::string& protocol) {
    for (auto& fac : streamConnectionFactories_) {
        if (fac.first == protocol) {
            throw Error(mocca::formatString("Factory for protocol '%%' already provided to selector", fac.first), __FILE__, __LINE__);
        }
    }
    streamConnectionFactories_.push_back({protocol, std::move(factory)});
}

void ConnectionFactorySelector::removeAll() {
    messageConnectionFactories_.clear();
    streamConnectionFactories_.clear();
}

void ConnectionFactorySelector::addDefaultFactories() {
    // tcp
    addStreamConnectionFactory(makeUnique<TCPConnectionFactory>(), tcp());

    // queue
    addStreamConnectionFactory(makeUnique<QueueConnectionFactory>(), queue());

    // tcp.prefixed
    addMessageConnectionFactory(makeUnique<FramingConnectionFactory>(std::unique_ptr<IStreamConnectionFactory>(new TCPConnectionFactory()),
                                                                     std::unique_ptr<FramingStrategy>(new SizePrefixedProtocol())),
                                tcpPrefixed());

    // queue.prefixed
    addMessageConnectionFactory(
        makeUnique<FramingConnectionFactory>(std::unique_ptr<IStreamConnectionFactory>(new QueueConnectionFactory()),
                                             std::unique_ptr<FramingStrategy>(new SizePrefixedProtocol())),
        queuePrefixed());

    // tcp.ws
    addMessageConnectionFactory(makeUnique<FramingConnectionFactory>(std::unique_ptr<IStreamConnectionFactory>(new TCPConnectionFactory()),
                                                                     std::unique_ptr<FramingStrategy>(new WebSocketProtocol())),
                                tcpWebSocket());

    // queue.ws
    addMessageConnectionFactory(
        makeUnique<FramingConnectionFactory>(std::unique_ptr<IStreamConnectionFactory>(new QueueConnectionFactory()),
                                             std::unique_ptr<FramingStrategy>(new WebSocketProtocol())),
        queueWebSocket());

    // loopback
    addMessageConnectionFactory(makeUnique<LoopbackConnectionFactory>(), loopback());
}

std::unique_ptr<IMessageConnectionAcceptor> ConnectionFactorySelector::bind(const Endpoint& endpoint) {
    auto& serv = messageConnectionFactory(endpoint.protocol);
    return serv.bind(endpoint.address());
}

std::unique_ptr<IMessageConnection> ConnectionFactorySelector::connect(const Endpoint& endpoint) {
    auto& serv = messageConnectionFactory(endpoint.protocol);
    return serv.connect(endpoint.address());
}

const std::string& ConnectionFactorySelector::tcp() {
    static std::string str = "tcp";
    return str;
}

const std::string& ConnectionFactorySelector::queue() {
    static std::string str = "queue";
    return str;
}

const std::string& ConnectionFactorySelector::loopback() {
    static std::string str = "loopback";
    return str;
}

const std::string& ConnectionFactorySelector::tcpPrefixed() {
    static std::string str = "tcp.prefixed";
    return str;
}

const std::string& ConnectionFactorySelector::tcpWebSocket() {
    static std::string str = "tcp.ws";
    return str;
}

const std::string& ConnectionFactorySelector::queuePrefixed() {
    static std::string str = "queue.prefixed";
    return str;
}

const std::string& ConnectionFactorySelector::queueWebSocket() {
    static std::string str = "queue.ws";
    return str;
}