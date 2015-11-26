#include "mocca/net/TCPConnectionListener.h"
#include "mocca/net/Sockets.h"

namespace mocca {
namespace net {

TCPConnectionListener::TCPConnectionListener(int port) 
{
    try {
        server_.SetReuseAddress(true);
        server_.SetNonBlocking(true);
        server_.Bind(IVDA::NetworkAddress(IVDA::NetworkAddress::Any, port));
        server_.Listen(3); // ???
    } catch (const IVDA::SocketException& err) {
        std::string internalError(err.what());
        throw NetworkError("Network error while binding to port (internal error: " + internalError + ")",
                           __FILE__, __LINE__);
    }
}

std::unique_ptr<AbstractConnection> TCPConnectionListener::getConnection(std::chrono::milliseconds timeout) {
    IVDA::TCPSocket* connectionSocket = nullptr;
    try {
        server_.AcceptNewConnection((IVDA::ConnectionSocket**)&connectionSocket, static_cast<uint32_t>(timeout.count()));
    }
    catch (const IVDA::SocketConnectionException&) {
        // a connection terminated before it could be accepted; presumably this is not a
        // critical situation, so we ignore the error
    }
    if (connectionSocket) {
        auto socketPtr = std::unique_ptr<IVDA::ConnectionSocket>(connectionSocket);
        TCPNetworkAddress address(connectionSocket->GetPeerAddress(),
            connectionSocket->GetPeerPort());
        return std::unique_ptr<TCPConnection>(
            new TCPConnection(address, move(socketPtr)));
    }
    return nullptr;
}

}
}