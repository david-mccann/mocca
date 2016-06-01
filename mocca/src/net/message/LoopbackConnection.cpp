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

#include "mocca/net/message/LoopbackConnection.h"

#include "mocca/net/NetworkError.h"

using namespace mocca;
using namespace mocca::net;

LoopbackConnection::LoopbackConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                                       std::shared_ptr<SQ> inSignalQueue, std::shared_ptr<const ConnectionID> connectionID)
    : connectionID_(connectionID)
    , connected_(true)
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

LoopbackConnection::~LoopbackConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::shared_ptr<const ConnectionID> LoopbackConnection::connectionID() const {
    return connectionID_;
}

bool LoopbackConnection::isConnected() const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull() && signal == Signal::Disconnect) {
        connected_ = false;
    }
    return connected_;
}

void LoopbackConnection::send(Message message) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer " + connectionID_->peerEndpoint.toString() + " has been closed", *connectionID_,
                                    __FILE__, __LINE__);
    }
    sendQueue_->enqueue(std::move(message));
}

Message LoopbackConnection::receive() const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer " + connectionID_->peerEndpoint.toString() + " has been closed", *connectionID_,
                                    __FILE__, __LINE__);
    }
    auto data = receiveQueue_->dequeue(std::chrono::milliseconds(200));
    return data.isNull() ? Message() : data.release();
}