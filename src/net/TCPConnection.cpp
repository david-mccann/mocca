#include "mocca/net/TCPConnection.h"

namespace mocca {
namespace net {

TCPConnection::TCPConnection(const TCPNetworkAddress& networkAddress,
                             std::unique_ptr<IVDA::ConnectionSocket> socket)
    : networkAddress_(networkAddress), socket_(move(socket)) {}

mocca::net::TCPConnection::~TCPConnection()
{
    if (socket_ != nullptr) {
        try {
            socket_->Disconnect();
            socket_->Close();
        }
        catch (IVDA::SocketException&) {
            // ignore exception; exceptions must not escape the destructor!
        }
    }
}

TCPNetworkAddress TCPConnection::networkAddress() const { return networkAddress_; }

void TCPConnection::send(ByteArray message) const {
    try {
        std::lock_guard<std::mutex> lock(sendMx_);
        socket_->SendInt(message.size());
        socket_->SendData((const int8_t*)message.data(), message.size());
    }
    catch (const IVDA::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() +
            " lost during send operation (internal error: " + err.what() + ")",
            __FILE__, __LINE__);
    }
    catch (const IVDA::SocketException& err) {
        std::string internalError(err.what());
        throw NetworkError("Network error in send operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}

ByteArray TCPConnection::receive(std::chrono::milliseconds timeout) const { 
    try {
        int messageSize;
        std::lock_guard<std::mutex> lock(receiveMx_);
        auto bytesRead = socket_->ReceiveInt(messageSize, static_cast<uint32_t>(timeout.count() / 2));
        if (bytesRead == sizeof(int)) {
            ByteArray message(messageSize);
            auto bytesRead = socket_->ReceiveData((int8_t*)message.data(), messageSize, static_cast<uint32_t>(timeout.count() / 2));
            if (bytesRead == messageSize) {
                message.setSize(messageSize);
                return message;
            }
        }
        return ByteArray();
    }
    catch (const IVDA::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() +
            " lost during receive operation (internal error: " + err.what() + ")",
            __FILE__, __LINE__);
    }
    catch (const IVDA::SocketException& err) {
        std::string internalErr(err.what());
        throw NetworkError("Network error in receive operation (internal error: " + internalErr + ")", __FILE__, __LINE__);
    }
}
}
}