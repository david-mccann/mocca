#include "mocca/net/TCPConnection.h"

#include "mocca/base/StringTools.h"

namespace mocca {
namespace net {

TCPConnection::TCPConnection(std::unique_ptr<IVDA::ConnectionSocket> socket)
    : socket_(move(socket))
    , identifier_(createIdentifier()) {}

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

std::string TCPConnection::identifier() const {
    return identifier_;
}

std::string TCPConnection::createIdentifier() {
    static unsigned int count = 0;
    ++count;
    return "tcp_" + std::to_string(count);
}

void TCPConnection::lock() {
    mutex_.lock();
}

void mocca::net::TCPConnection::unlock() {
    mutex_.unlock();
}

void TCPConnection::send(ByteArray message, std::chrono::milliseconds timeout) const {
    try {
        socket_->SendData((const int8_t*)message.data(), message.size(), static_cast<uint32_t>(timeout.count()));
    } catch (const IVDA::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() + " lost during send operation (internal error: " +
                                        err.what() + ")",
                                    identifier(), __FILE__, __LINE__);
    } catch (const IVDA::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in send operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}

ByteArray TCPConnection::receive(uint32_t maxSize, std::chrono::milliseconds timeout) const {
    try {
        ByteArray message(maxSize);
        auto bytesRead = socket_->ReceiveData((int8_t*)message.data(), maxSize, static_cast<uint32_t>(timeout.count()));
        message.setSize(bytesRead);
        return message;
    } catch (const IVDA::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() + " lost during receive operation (internal error: " +
                                        err.what() + ")",
                                    identifier(), __FILE__, __LINE__);
    } catch (const IVDA::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in receive operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}
}
}