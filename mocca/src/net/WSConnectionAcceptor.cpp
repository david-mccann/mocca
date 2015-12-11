#include "mocca/net/WSConnectionAcceptor.h"

#include "mocca/net/PhysicalConnection.h"
#include "mocca/net/stream/TCPConnectionAcceptor.h"
#include "mocca/net/WSConnection.h"
#include "mocca/net/WSHandshake.h"
#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"

#include "mocca/log/LogManager.h"

using namespace mocca;
using namespace mocca::net;

template <typename PhysicalConnectionAcceptorType>
WSConnectionAcceptor<PhysicalConnectionAcceptorType>::WSConnectionAcceptor(
    std::unique_ptr<PhysicalConnectionAcceptorType> physicalConnectionAcceptor)
    : physicalConnectionAcceptor_(std::move(physicalConnectionAcceptor)) {}

template <typename PhysicalConnectionAcceptorType>
std::unique_ptr<IProtocolConnection>
WSConnectionAcceptor<PhysicalConnectionAcceptorType>::getConnection(std::chrono::milliseconds timeout) {
    auto physicalConnection = physicalConnectionAcceptor_->getConnection(timeout);
    if (physicalConnection == nullptr) {
        return nullptr;
    }
    auto handshake = receiveUntil(*physicalConnection, "\r\n\r\n");
    std::string handshakeStr((char*)handshake.data(), handshake.size());
    auto connectionInfo = mocca::net::parseWSHandshake(handshakeStr);

    auto responseStr = mocca::net::createWSHandshakeResponse(connectionInfo);
    ByteArray handshakeResponse = ByteArray::createFromRaw(responseStr.c_str(), responseStr.size());
    physicalConnection->send(std::move(handshakeResponse));

    return std::unique_ptr<IProtocolConnection>(
        new WSConnection<typename PhysicalConnectionAcceptorType::ConnectionType>(std::move(physicalConnection), connectionInfo));
}


template class WSConnectionAcceptor<TCPConnectionAcceptor>;
template class WSConnectionAcceptor<LoopbackPhysicalConnectionAcceptor>;