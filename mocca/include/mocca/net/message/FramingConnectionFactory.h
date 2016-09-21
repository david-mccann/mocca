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

#pragma once

#include "mocca/net/IMessageConnectionFactory.h"
#include "mocca/net/IStreamConnectionFactory.h"
#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class FramingConnectionFactory : public IMessageConnectionFactory {
public:
    FramingConnectionFactory(std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory,
                             std::unique_ptr<FramingStrategy> framingStrategy);

    std::unique_ptr<IMessageConnection> connect(const std::string& address) override;
    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& machine, const std::string& port) override;

    IStreamConnectionFactory& streamConnectionFactory();

private:
    std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory_;
    std::unique_ptr<FramingStrategy> framingStrategy_;
};
}
}
