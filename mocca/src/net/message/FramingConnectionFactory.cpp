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

#include "mocca/net/message/FramingConnectionFactory.h"

#include "mocca/net/message/FramingConnection.h"
#include "mocca/net/message/FramingConnectionAcceptor.h"

using namespace mocca::net;

FramingConnectionFactory::FramingConnectionFactory(std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory,
                                                   std::unique_ptr<FramingStrategy> framingStrategy)
    : streamConnectionFactory_(std::move(streamConnectionFactory))
    , framingStrategy_(std::move(framingStrategy)) {}

std::unique_ptr<IMessageConnection> FramingConnectionFactory::connect(const std::string& address) {
    return std::unique_ptr<IMessageConnection>(
        new FramingConnection(streamConnectionFactory_->connect(address), framingStrategy_->clone()));
}

std::unique_ptr<IMessageConnectionAcceptor> FramingConnectionFactory::bind(const std::string& machine, const std::string& port) {
    return std::unique_ptr<IMessageConnectionAcceptor>(
        new FramingConnectionAcceptor(streamConnectionFactory_->bind(machine, port), framingStrategy_->clone()));
}

IStreamConnectionFactory& mocca::net::FramingConnectionFactory::streamConnectionFactory() {
    return *streamConnectionFactory_;
}