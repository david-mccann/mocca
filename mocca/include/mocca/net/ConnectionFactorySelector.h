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
#include "mocca/net/IMessageConnectionFactory.h"
#include "mocca/net/IStreamConnectionFactory.h"

#include <memory>
#include <vector>

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