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

std::unique_ptr<IStreamConnectionAcceptor> QueueConnectionFactory::bind(const std::string& name) {
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