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

#include "mocca/base/Thread.h"
#include "mocca/net/message/FramingConnection.h"

using namespace mocca;
using namespace mocca::net;

FramingConnection::FramingConnection(std::unique_ptr<IStreamConnection> stream, std::unique_ptr<FramingStrategy> framingStrategy)
    : stream_(std::move(stream))
    , framingStrategy_(std::move(framingStrategy)) {
    auto localEndpont = stream_->connectionID()->localEndpoint;
    auto peerEndpoint = stream_->connectionID()->peerEndpoint;

    localEndpont.protocol = localEndpont.protocol + "." + framingStrategy_->name();
    peerEndpoint.protocol = peerEndpoint.protocol + "." + framingStrategy_->name();

    connectionID_ = std::make_shared<ConnectionID>(localEndpont, peerEndpoint);
}

std::shared_ptr<const ConnectionID> FramingConnection::connectionID() const {
    return stream_->connectionID();
}

bool FramingConnection::isConnected() const {
    return stream_->isConnected();
}

void FramingConnection::send(Message message) const {
    try {
        framingStrategy_->writeMessageToStream(*stream_, std::move(message));
    } catch (const ThreadInterrupt&) {
    }
}

Message FramingConnection::receive() const {
    try {
        return framingStrategy_->readMessageFromStream(*stream_);
    } catch (const ThreadInterrupt&) {
        return Message();
    }
}