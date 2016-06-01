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

#include "mocca/net/message/FramingConnectionAcceptor.h"

#include "mocca/net/message/FramingConnection.h"

using namespace mocca::net;

FramingConnectionAcceptor::FramingConnectionAcceptor(std::unique_ptr<IStreamConnectionAcceptor> streamAcceptor,
                                                     std::unique_ptr<FramingStrategy> framingStrategy)
    : streamAcceptor_(std::move(streamAcceptor))
    , framingStrategy_(std::move(framingStrategy)) {
    auto endpoint = *streamAcceptor_->localEndpoint();
    endpoint.protocol = endpoint.protocol + "." + framingStrategy_->name();
    endpoint_ = std::make_shared<Endpoint>(endpoint);
}

std::unique_ptr<IMessageConnection> FramingConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    auto stream = streamAcceptor_->accept(timeout);
    if (stream != nullptr) {
        auto strategyCopy = framingStrategy_->clone();
        strategyCopy->performHandshake(*stream);
        return std::unique_ptr<IMessageConnection>(new FramingConnection(std::move(stream), std::move(strategyCopy)));
    }
    return nullptr;
}

std::shared_ptr<const Endpoint> FramingConnectionAcceptor::localEndpoint() const {
    return endpoint_;
}