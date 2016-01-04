#include "mocca/net/stream/TCPConnection.h"

#include "mocca/base/StringTools.h"

namespace mocca {
namespace net {

TCPConnection::TCPConnection(std::unique_ptr<IVDA::ConnectionSocket> socket)
    : socket_(move(socket)) {}

TCPConnection::~TCPConnection() {
    if (socket_ != nullptr) {
        try {
            socket_->Disconnect();
            socket_->Close();
        } catch (IVDA::SocketException&) {
            // ignore exception; exceptions must not escape the destructor!
        }
    }
}

std::shared_ptr<const ConnectionID> TCPConnection::connectionID() const {
    if (connectionID_ == nullptr) {
        std::string localIP, peerIP;
        uint16_t localPort, peerPort;
        socket_->GetLocalNetworkAddress().GetAddress(localIP, localPort);
        socket_->GetPeerNetworkAddress().GetAddress(peerIP, peerPort);
        connectionID_ = std::make_shared<ConnectionID>(Endpoint("tcp", localIP, std::to_string(localPort)),
                                                       Endpoint("tcp", peerIP, std::to_string(peerPort)));
    }
    return connectionID_;
}

void TCPConnection::send(ByteArray message, std::chrono::milliseconds timeout) const {
    try {
        socket_->SendData((const int8_t*)message.data(), message.size(), static_cast<uint32_t>(timeout.count()));
    } catch (const IVDA::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() + " lost during send operation (internal error: " +
                                        err.what() + ")",
                                    *connectionID(), __FILE__, __LINE__);
    } catch (const IVDA::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in send operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}

ByteArray TCPConnection::readFromStream(uint32_t maxSize, std::chrono::milliseconds timeout) const {
    try {
        ByteArray message(maxSize);
        auto bytesRead = socket_->ReceiveData((int8_t*)message.data(), maxSize, static_cast<uint32_t>(timeout.count()));
        message.setSize(bytesRead);
        return message;
    } catch (const IVDA::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() + " lost during receive operation (internal error: " +
                                        err.what() + ")",
                                    *connectionID(), __FILE__, __LINE__);
    } catch (const IVDA::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in receive operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}
}
}