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

#include "mocca/net/stream/QueueConnection.h"

#include "mocca/net/NetworkError.h"


using namespace mocca;
using namespace mocca::net;

QueueConnection::QueueConnection(std::shared_ptr<MQ> sendQueue, std::shared_ptr<MQ> receiveQueue, std::shared_ptr<SQ> outSignalQueue,
                                 std::shared_ptr<SQ> inSignalQueue, std::shared_ptr<const ConnectionID> connectionID)
    : connectionID_(connectionID)
    , connected_(true)
    , sendQueue_(sendQueue)
    , receiveQueue_(receiveQueue)
    , outSignalQueue_(outSignalQueue)
    , inSignalQueue_(inSignalQueue) {}

QueueConnection::~QueueConnection() {
    outSignalQueue_->enqueue(Signal::Disconnect);
}

std::shared_ptr<const ConnectionID> QueueConnection::connectionID() const {
    return connectionID_;
}

bool QueueConnection::isConnected() const {
    auto signal = inSignalQueue_->dequeueNoWait();
    if (!signal.isNull() && signal == Signal::Disconnect) {
        connected_ = false;
    }
    return connected_;
}

uint32_t QueueConnection::send(const uint8_t* data, uint32_t size, std::chrono::milliseconds timeout) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
    }
    auto dataIt = data;
    auto dataEnd = data + size;
    while (dataIt != dataEnd) {
        sendQueue_->enqueue(*dataIt);
        ++dataIt;
    }
    return size;
}

uint32_t QueueConnection::receive(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout) const {
    if (!isConnected()) {
        throw ConnectionClosedError("Connection to peer has been closed", *connectionID_, __FILE__, __LINE__);
    }
    for (uint32_t i = 0; i < maxSize; ++i) {
        auto dataNullable = receiveQueue_->dequeue(timeout);
        if (dataNullable.isNull()) {
            return i;
        }
        auto data = dataNullable.get();
        buffer[i] = data;
    }
    return maxSize;
}
