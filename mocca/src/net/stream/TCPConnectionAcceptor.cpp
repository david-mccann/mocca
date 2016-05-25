/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/stream/TCPConnectionAcceptor.h"

#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/stream/TCPConnection.h"

namespace mocca {
namespace net {

TCPConnectionAcceptor::TCPConnectionAcceptor(int port) {
    try {
        server_.SetReuseAddress(true);
        server_.SetNonBlocking(true);
        server_.Bind(IVDB::NetworkAddress(IVDB::NetworkAddress::Any, port));
        server_.Listen(3); // ???
        endpoint_ = std::make_shared<Endpoint>("tcp", "*", std::to_string(server_.GetLocalPort()));
    } catch (const IVDB::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error while binding to port (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}

std::unique_ptr<IStreamConnection> TCPConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    IVDB::TCPSocket* connectionSocket = nullptr;
    try {
        server_.AcceptNewConnection((IVDB::ConnectionSocket**)&connectionSocket, static_cast<uint32_t>(timeout.count()));
    } catch (const IVDB::SocketConnectionException&) {
        // a connection terminated before it could be accepted; presumably this is not a
        // critical situation, so we ignore the error
    }
    if (connectionSocket) {
        auto socketPtr = std::unique_ptr<IVDB::ConnectionSocket>(connectionSocket);
        auto ip = connectionSocket->GetPeerAddress();
        LDEBUG("Accepted TCP connection on local endpoint " << *endpoint_);
        return std::unique_ptr<IStreamConnection>(new TCPConnection(std::move(socketPtr)));
    }
    return nullptr;
}

std::shared_ptr<const Endpoint> TCPConnectionAcceptor::localEndpoint() const {
    return endpoint_;
}
}
}