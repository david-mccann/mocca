/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include "mocca/net/stream/TCPConnection.h"

#include "mocca/base/StringTools.h"

namespace mocca {
namespace net {

TCPConnection::TCPConnection(std::unique_ptr<IVDB::ConnectionSocket> socket)
    : socket_(move(socket)) {
    socket_->SetNonBlocking(true);
    socket_->SetNoSigPipe(true); // disable sigpipe to prevent crashes
    socket_->SetNoDelay(true);   // disable Nagle algorithm for low-latency transmission

    std::string localIP, peerIP;
    uint16_t localPort, peerPort;
    socket_->GetLocalNetworkAddress().GetAddress(localIP, localPort);
    socket_->GetPeerNetworkAddress().GetAddress(peerIP, peerPort);
    connectionID_ = std::make_shared<ConnectionID>(Endpoint("tcp", localIP, std::to_string(localPort)),
                                                   Endpoint("tcp", peerIP, std::to_string(peerPort)));
}

TCPConnection::~TCPConnection() {
    if (socket_ != nullptr) {
        try {
            socket_->Disconnect();
            socket_->Close();
        } catch (IVDB::SocketException&) {
            // ignore exception; exceptions must not escape the destructor!
        }
    }
}

std::shared_ptr<const ConnectionID> TCPConnection::connectionID() const {
    return connectionID_;
}

bool TCPConnection::isConnected() const {
    return socket_->IsConnected();
}

uint32_t TCPConnection::send(const uint8_t* data, uint32_t size, std::chrono::milliseconds timeout) const {
    try {
        return socket_->SendData(reinterpret_cast<const int8_t*>(data), size, static_cast<uint32_t>(timeout.count()));
    } catch (const IVDB::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + connectionID_->peerEndpoint.toString() +
                                        " lost during send operation (internal error: " + err.what() + ")",
                                    *connectionID_, __FILE__, __LINE__);
    } catch (const IVDB::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in send operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}

uint32_t TCPConnection::receive(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout) const {
    try {
        return socket_->ReceiveData(reinterpret_cast<int8_t*>(buffer), maxSize, static_cast<uint32_t>(timeout.count()));
    } catch (const IVDB::SocketConnectionException& err) {
        throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() + " lost during receive operation (internal error: " +
                                        err.what() + ")",
                                    *connectionID(), __FILE__, __LINE__);
    } catch (const IVDB::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in receive operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}
}
}
