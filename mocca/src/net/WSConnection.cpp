#include "mocca/net/WSConnection.h"

#include "mocca/net/PhysicalConnection.h"

#include <mutex>

using namespace mocca;
using namespace mocca::net;

#define MOCCA_CHECK_WS_FRAME

/*
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
*/

WSConnection::WSConnection(std::unique_ptr<IPhysicalConnection> physicalConnection, const WSConnectionInfo& connectionInfo)
    : physicalConnection_(std::move(physicalConnection))
    , connectionInfo_(connectionInfo) {}

std::string mocca::net::WSConnection::identifier() const {
    return physicalConnection_->identifier();
}

void WSConnection::send(ByteArray message) const {}

ByteArray WSConnection::receive(std::chrono::milliseconds timeout) const {
    std::lock_guard<IPhysicalConnection> lock(*physicalConnection_);
    auto data = receiveExactly(*physicalConnection_, 2, timeout); // 2 bytes metadata + 4 bytes mask
#ifdef MOCCA_CHECK_WS_FRAME
    if (data[0] != 0x81) { // final fragment, text frame
        throw Error("Invalid WebSocket frame: Unsupported or malformed", __FILE__, __LINE__);
    }
#endif

    unsigned char basicSize = data[1] & 0x7F;
    unsigned long payloadSize = 0;
    int masksOffset = 2;
    if (basicSize <= 125) {
        payloadSize = basicSize;
        data.append(receiveExactly(*physicalConnection_, 4, timeout)); // 4 bytes mask
    } else if (basicSize == 126) {
        const int numPayloadBytes = 2;
        data.append(receiveExactly(*physicalConnection_, 6, timeout)); // 2 bytes payload length + 4 bytes mask
        for (int c = 0; c < numPayloadBytes; ++c) {
            payloadSize += data[c + 2] << (8 * (numPayloadBytes - 1 - c));
        }
        masksOffset = 4;
    } else if (basicSize == 127) {
        const int numPayloadBytes = 8;
        data.append(receiveExactly(*physicalConnection_, 12, timeout)); // 8 bytes payload length + 4 bytes mask
        for (int c = 0; c < numPayloadBytes; ++c) {
            payloadSize += data[c + 2] << (8 * (numPayloadBytes - 1 - c));
        }
        masksOffset = 10;
    }

    unsigned char mask[4];
    memcpy(mask, data.data() + masksOffset, 4);

    ByteArray payload = receiveExactly(*physicalConnection_, payloadSize, timeout);
    for (unsigned long int i = 0; i < payloadSize; ++i) {
        payload[i] = (payload[i] ^ mask[i % 4]);
    }
    return payload;
}

WSConnectionInfo mocca::net::WSConnection::connectionInfo() const {
    return connectionInfo_;
}