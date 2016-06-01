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

#include "mocca/net/stream/QueueConnectionFactory.h"

#include "mocca/net/NetworkError.h"
#include "mocca/net/stream/QueueConnectionAcceptor.h"

using namespace mocca::net;


std::unique_ptr<IStreamConnection> QueueConnectionFactory::connect(const std::string& name) {
    auto spawner = getSpawner(name);
    if (spawner == nullptr) {
        throw NetworkError("No connection acceptor bound to name " + name, __FILE__, __LINE__);
    }
    return spawner->getClientConnection();
}

std::unique_ptr<IStreamConnectionAcceptor> QueueConnectionFactory::bind(const std::string& machine, const std::string& port) {
    static int autoPortCount = 0;
    std::string name = machine + ":" + (port == Endpoint::autoPort() ? std::to_string(autoPortCount++) : port); 
    auto spawner = getSpawner(name);
    if (spawner == nullptr) {
        spawner = std::make_shared<QueueConnectionSpawner>(name);
        spawners_.push_back(spawner);
    }
    return std::unique_ptr<IStreamConnectionAcceptor>(new QueueConnectionAcceptor(spawner));
}

std::shared_ptr<QueueConnectionSpawner> mocca::net::QueueConnectionFactory::getSpawner(const std::string& name) {
    for (const auto& spawner : spawners_) {
        if (spawner->name() == name) {
            return spawner;
        }
    }
    return nullptr;
}