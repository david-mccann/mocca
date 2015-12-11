#include "mocca/net/WSConnection.h"

#include "mocca/base/Endian.h"
#include "mocca/net/Error.h"
#include "mocca/net/PhysicalConnection.h"
#include "mocca/net/stream/TCPConnection.h"
#include "mocca/testing/LoopbackPhysicalConnection.h"

#include <limits>
#include <mutex>
#include <sstream>

using namespace mocca;
using namespace mocca::net;

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


template <typename PhysicalConnectionType>
WSConnection<PhysicalConnectionType>::WSConnection(std::unique_ptr<PhysicalConnectionType> physicalConnection,
                                                   const WSConnectionInfo& connectionInfo)
    : physicalConnection_(std::move(physicalConnection))
    , connectionInfo_(connectionInfo) {}

template <typename PhysicalConnectionType> std::string WSConnection<PhysicalConnectionType>::identifier() const {
    return physicalConnection_->identifier();
}

template <typename PhysicalConnectionType> void WSConnection<PhysicalConnectionType>::send(ByteArray message) const {
    auto payloadSize = message.size();
    ByteArray sendBuffer(payloadSize + 10); // header size is at most 10 bytes

    // create the flags byte
    unsigned char payloadFlags = 0x81;
    sendBuffer.append(&payloadFlags, 1);

    // create the length bytes
    if (payloadSize <= 125) {
        char basicSize = payloadSize;
        sendBuffer.append(&basicSize, 1);
    } else if (payloadSize > 125 && payloadSize <= 65535) {
        char basicSize = 126;
        sendBuffer.append(&basicSize, 1);
        uint16_t len = swap_uint16(static_cast<uint16_t>(payloadSize));
        sendBuffer.append(&len, 2);
    } else {
        char basicSize = 127;
        sendBuffer.append(&basicSize, 1);
        uint64_t len = swap_uint64(static_cast<uint64_t>(payloadSize));
        sendBuffer.append(&len, 8);
    }

    // append payload
    sendBuffer.append(message);

    physicalConnection_->send(std::move(sendBuffer));
}

template <typename PhysicalConnectionType>
ByteArray WSConnection<PhysicalConnectionType>::receive(std::chrono::milliseconds timeout) const {
    std::lock_guard<PhysicalConnectionType> lock(*physicalConnection_);

    // read the flags byte
    auto data = receiveExactly(*physicalConnection_, 1, timeout);
    if (data.isEmpty()) {
        return ByteArray();
    }
#ifdef MOCCA_RUNTIME_CHECKS
    if (data[0] != 0x81 && data[0] != 0x88) { // 0x81 = final fragment, text frame; 0x88 = final fragment, close connection
        std::stringstream stream;
        stream << std::hex << static_cast<int>(data[0]);
        throw Error("Invalid WebSocket frame: unsupported or malformed (flag byte: " + stream.str() + ")", __FILE__, __LINE__);
    }
#endif

    if (data[0] == 0x88) {
        throw ConnectionClosedError("WebSocket connection closed", identifier(), __FILE__, __LINE__);
    }

    // read the basic payload byte
    data.append(receiveExactly(*physicalConnection_, 1, timeout));

    // read additional payload-size bytes and the mask bytes
    unsigned char basicSize = data[1] & 0x7F;
    uint64_t payloadSize = 0;
    int maskOffset = 2;
    if (basicSize <= 125) {
        payloadSize = basicSize;
        data.append(receiveExactly(*physicalConnection_, 4, timeout)); // 4 bytes mask
    } else if (basicSize == 126) {
        data.append(receiveExactly(*physicalConnection_, 6, timeout)); // 2 bytes payload length + 4 bytes mask
        payloadSize = swap_uint16(*reinterpret_cast<uint16_t*>(data.data() + 2));
        maskOffset = 4;
    } else if (basicSize == 127) {
        data.append(receiveExactly(*physicalConnection_, 12, timeout)); // 8 bytes payload length + 4 bytes mask
        payloadSize = swap_uint64(*reinterpret_cast<uint64_t*>(data.data() + 2));
#ifdef MOCCA_RUNTIME_CHECKS
        if (payloadSize > std::numeric_limits<uint32_t>::max()) {
            throw Error("Invalid WebSocket frame: frame size exceeds buffer size", __FILE__, __LINE__);
        }
#endif
        maskOffset = 10;
    }
#ifdef MOCCA_RUNTIME_CHECKS
    else {
        throw Error("Invalid WebSocket frame: malformed payload-size", __FILE__, __LINE__);
    }
#endif

    // read and unmask payload data
    ByteArray payload = receiveExactly(*physicalConnection_, static_cast<uint32_t>(payloadSize), timeout);
    unsigned char* mask = data.data() + maskOffset;
    for (unsigned long int i = 0; i < payloadSize; ++i) {
        payload[i] ^= mask[i % 4];
    }
    return payload;
}

template <typename PhysicalConnectionType> WSConnectionInfo WSConnection<PhysicalConnectionType>::connectionInfo() const {
    return connectionInfo_;
}


template class WSConnection<TCPConnection>;
template class WSConnection<LoopbackPhysicalConnection>;