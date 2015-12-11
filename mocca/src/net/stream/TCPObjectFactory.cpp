#include "mocca/net/stream/TCPObjectFactory.h"

#include "mocca/base/StringTools.h"
#include "mocca/net/stream/TCPNetworkAddress.h"

using namespace mocca::net;

std::unique_ptr<TCPStream> TCPObjectFactory::createStreamImpl(const std::string& args) {
    TCPNetworkAddress networkAddress(args);
    auto socket = std::unique_ptr<IVDA::TCPSocket>(new IVDA::TCPSocket());
    socket->SetNonBlocking(true);
    try {
        socket->Connect(IVDA::NetworkAddress(networkAddress.ip(), networkAddress.port()));
    } catch (const IVDA::SocketConnectionException& err) {
        throw ConnectFailedError("Could not connect to " + networkAddress.toString() + " (internal error: " + err.what() + ")", __FILE__,
                                 __LINE__);
    } catch (const IVDA::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in connect operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
    return std::unique_ptr<TCPStream>(new TCPStream(std::move(socket)));
}

std::unique_ptr<TCPStreamAcceptor> TCPObjectFactory::createAcceptorImpl(const std::string& args) {
    int port = TCPNetworkAddress::parsePort(args);
    return std::unique_ptr<TCPStreamAcceptor>(new TCPStreamAcceptor(port));
}