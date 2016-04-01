/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/stream/TCPConnectionFactory.h"

#include "mocca/base/StringTools.h"
#include "mocca/net/stream/TCPConnection.h"
#include "mocca/net/stream/TCPConnectionAcceptor.h"
#include "mocca/net/stream/TCPNetworkAddress.h"

using namespace mocca::net;

std::unique_ptr<IStreamConnection> TCPConnectionFactory::connect(const std::string& address) {
    TCPNetworkAddress networkAddress(address);
    auto socket = std::unique_ptr<IVDB::TCPSocket>(new IVDB::TCPSocket());
    try {
        socket->Connect(IVDB::NetworkAddress(networkAddress.ip(), networkAddress.port()));
    } catch (const IVDB::SocketConnectionException& err) {
        throw ConnectFailedError("Could not connect to " + networkAddress.toString() + " (internal error: " + err.what() + ")", __FILE__,
                                 __LINE__);
    } catch (const IVDB::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in connect operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
    return std::unique_ptr<IStreamConnection>(new TCPConnection(std::move(socket)));
}

std::unique_ptr<IStreamConnectionAcceptor> TCPConnectionFactory::bind(const std::string& machine, const std::string& port) {
    TCPNetworkAddress tcpAddress(machine, port == Endpoint::autoPort() ? 0 : std::stoi(port));
    return std::unique_ptr<IStreamConnectionAcceptor>(new TCPConnectionAcceptor(tcpAddress.port()));
}