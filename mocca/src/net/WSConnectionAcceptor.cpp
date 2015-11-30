#include "mocca/net/WSConnectionAcceptor.h"

#include "mocca/net/PhysicalConnection.h"
#include "mocca/net/WSConnection.h"
#include "mocca/net/WSHandshake.h"

#include "mocca/log/LogManager.h"

using namespace mocca;
using namespace mocca::net;

WSConnectionAcceptor::WSConnectionAcceptor(std::unique_ptr<IPhysicalConnectionAcceptor> physicalConnectionAcceptor)
    : physicalConnectionAcceptor_(std::move(physicalConnectionAcceptor)) {}

std::unique_ptr<IProtocolConnection> WSConnectionAcceptor::getConnection(std::chrono::milliseconds timeout) {
    auto physicalConnection = physicalConnectionAcceptor_->getConnection(timeout);
    if (physicalConnection == nullptr) {
        return nullptr;
    }
    auto header = receiveUntil(*physicalConnection, "\r\n\r\n");
    std::string headerStr((char*)header.data(), header.size());
    auto connectionInfo = mocca::net::parseWSHandshake(headerStr);

    ByteArray handshakeResponse;
    handshakeResponse << mocca::net::createWSHandshakeResponse(connectionInfo);
    physicalConnection->send(handshakeResponse);

    return std::unique_ptr<IProtocolConnection>(new WSConnection(std::move(physicalConnection), connectionInfo));
}