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

#include "mocca/net/message/LoopbackConnectionSpawner.h"

using namespace mocca::net;

LoopbackConnectionSpawner::LoopbackConnectionSpawner(const std::string& name)
    : name_(name) {}

std::unique_ptr<IMessageConnection> LoopbackConnectionSpawner::getClientConnection() {
    auto messageQueue1 = std::make_shared<LoopbackConnection::MQ>();
    auto messageQueue2 = std::make_shared<LoopbackConnection::MQ>();

    auto signalQueue1 = std::make_shared<LoopbackConnection::SQ>();
    auto signalQueue2 = std::make_shared<LoopbackConnection::SQ>();

    static unsigned int connectionCounter = 0;
    Endpoint ep1("loopback", "local", "#" + std::to_string(connectionCounter++));
    Endpoint ep2("loopback", "local", "#" + std::to_string(connectionCounter++));
    std::shared_ptr<const ConnectionID> serverID(std::make_shared<const ConnectionID>(ep1, ep2));
    std::shared_ptr<const ConnectionID> clientID(std::make_shared<const ConnectionID>(ep2, ep1));
    std::unique_ptr<LoopbackConnection> serverConnection(
        new LoopbackConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2, serverID));
    std::unique_ptr<IMessageConnection> clientConnection(
        new LoopbackConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1, clientID));

    serverConnections_.enqueue(std::move(serverConnection));
    return clientConnection;
}

std::unique_ptr<IMessageConnection> LoopbackConnectionSpawner::getServerConnection(std::chrono::milliseconds timeout) {
    auto connectionNullable = serverConnections_.dequeue(timeout);
    if (connectionNullable.isNull()) {
        return nullptr;
    }
    return connectionNullable.release();
}

std::string mocca::net::LoopbackConnectionSpawner::name() const {
    return name_;
}