#include "mocca/net/stream/TCPConnectionFactory.h"

#include "mocca/base/StringTools.h"
#include "mocca/net/stream/TCPConnection.h"
#include "mocca/net/stream/TCPConnectionAcceptor.h"
#include "mocca/net/stream/TCPNetworkAddress.h"

using namespace mocca::net;

std::unique_ptr<IStreamConnection> TCPConnectionFactory::connect(const std::string& address) {
    TCPNetworkAddress networkAddress(address);
    auto socket = std::unique_ptr<IVDA::TCPSocket>(new IVDA::TCPSocket());
    try {
        socket->Connect(IVDA::NetworkAddress(networkAddress.ip(), networkAddress.port()));
    } catch (const IVDA::SocketConnectionException& err) {
        throw ConnectFailedError("Could not connect to " + networkAddress.toString() + " (internal error: " + err.what() + ")", __FILE__,
                                 __LINE__);
    } catch (const IVDA::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in connect operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
    return std::unique_ptr<IStreamConnection>(new TCPConnection(std::move(socket)));
}

std::unique_ptr<IStreamConnectionAcceptor> TCPConnectionFactory::bind(const std::string& address) {
    TCPNetworkAddress tcpAddress(address);
    return std::unique_ptr<IStreamConnectionAcceptor>(new TCPConnectionAcceptor(tcpAddress.port()));
}