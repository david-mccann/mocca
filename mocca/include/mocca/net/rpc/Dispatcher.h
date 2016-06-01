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

#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/rpc/Method.h"

#include "jsoncpp/json.h"

#include <vector>

namespace mocca {
namespace net {

class Dispatcher : public mocca::Runnable {
public:
    Dispatcher(const std::vector<mocca::net::Endpoint>& endpoints);
    ~Dispatcher();

    void registerMethod(Method method);

private:
    void run() override;
    void sendReply(const JsonCpp::Value root, const std::vector<mocca::net::MessagePart>& binary,
                   std::shared_ptr<const mocca::net::ConnectionID> connectionID);
    JsonCpp::Value parseMessage(const mocca::net::Message& message);
    void registerReflection();

private:
    std::unique_ptr<mocca::net::ConnectionAggregator> aggregator_;
    std::vector<Method> methods_;
};
}
}
