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

#include "mocca/net/stream/TCPConnectionFactory.h"

#include "mocca/base/StringTools.h"
#include "mocca/net/stream/TCPConnection.h"
#include "mocca/net/stream/TCPConnectionAcceptor.h"
#include "mocca/net/stream/TCPNetworkAddress.h"

using namespace mocca::net;

std::unique_ptr<IStreamConnection> TCPConnectionFactory::connect(const std::string& address) {
    TCPNetworkAddress networkAddress(address);
    auto socket = std::unique_ptr<IVDB::TCPSocket>(new IVDB::TCPSocket());
    try {
        socket->Connect(IVDB::NetworkAddress(networkAddress.ip(), networkAddress.port()));
    } catch (const IVDB::SocketConnectionException& err) {
        throw ConnectFailedError("Could not connect to " + networkAddress.toString() + " (internal error: " + err.what() + ")", __FILE__,
                                 __LINE__);
    } catch (const IVDB::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error in connect operation (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
    return std::unique_ptr<IStreamConnection>(new TCPConnection(std::move(socket)));
}

std::unique_ptr<IStreamConnectionAcceptor> TCPConnectionFactory::bind(const std::string& machine, const std::string& port) {
    TCPNetworkAddress tcpAddress(machine, port == Endpoint::autoPort() ? 0 : std::stoi(port));
    return std::unique_ptr<IStreamConnectionAcceptor>(new TCPConnectionAcceptor(tcpAddress.port()));
}