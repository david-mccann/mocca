#include "mocca/net/WSConnectionAcceptor.h"

#include "mocca/net/PhysicalConnection.h"
#include "mocca/net/WSConnection.h"
#include "mocca/net/WebsocketProtocol.h"

#include "mocca/log/LogManager.h"

using namespace mocca;
using namespace mocca::net;

WSConnectionAcceptor::WSConnectionAcceptor(std::unique_ptr<IPhysicalConnectionAcceptor> physicalConnectionAcceptor)
    : physicalConnectionAcceptor_(std::move(physicalConnectionAcceptor)) {}

std::unique_ptr<IProtocolConnection> WSConnectionAcceptor::getConnection(std::chrono::milliseconds timeout) {
    auto physicalConnection = physicalConnectionAcceptor_->getConnection(timeout);
    if (physicalConnection == nullptr) {
        return nullptr;
    }
    auto header = receiveUntil(*physicalConnection, "\r\n\r\n");
    std::string headerStr(header.data(), header.size());
    LINFO("Websocket header received: " << headerStr);
    WebsocketProtocol wsProtocol;
    unsigned int count;
    IncomingPacket* wsMsg = nullptr;
    if (wsProtocol.incomingMessage(header, count, wsMsg) == WebsocketProtocol::Result::Success) {
        LINFO("Header is ok");
        WebsocketHandshakeMessage* request = (WebsocketHandshakeMessage*)wsMsg;
        if (request->Parse()) {
            std::unique_ptr<WebsocketHandshakeMessage> pResponse(new WebsocketHandshakeMessage());
            if (WebsocketProtocol::ProcessHandshake(*request, *pResponse)) {
                std::string response = pResponse->Serialize();
                ByteArray handshakeResponse;
                handshakeResponse.append(response.c_str(), response.length());
                LINFO("Sending response " << response);
                physicalConnection->send(handshakeResponse);
            }
        }
    }
    return std::unique_ptr<IProtocolConnection>(new WSConnection(std::move(physicalConnection)));
}