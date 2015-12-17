#include "mocca/net/framing/WebSocketProtocol.h"

#include "mocca/base/Endian.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/framing/FramingUtils.h"

#include "base64/base64.h"
#include "sha1/sha1.h"

using namespace mocca;
using namespace mocca::net;

std::unique_ptr<FramingStrategy> WebSocketProtocol::clone() const {
    return std::unique_ptr<FramingStrategy>(new WebSocketProtocol(*this));
}

void WebSocketProtocol::performHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    receiveHandshake(connection, timeout);
    sendHandshakeResponse(connection, timeout);
}

void mocca::net::WebSocketProtocol::receiveHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    auto handshake = readUntil(connection, "\r\n\r\n");
    std::string handshakeStr((char*)handshake.data(), handshake.size());
    std::stringstream stream(handshakeStr);
    std::string line;
    getline(stream, line);
    size_t methodEnd;
    if ((methodEnd = line.find(' ')) != std::string::npos) {
        size_t pathEnd;
        if ((pathEnd = line.find(' ', methodEnd + 1)) != std::string::npos) {
            connectionInfo_.method = line.substr(0, methodEnd);
            connectionInfo_.path = line.substr(methodEnd + 1, pathEnd - methodEnd - 1);
            if ((pathEnd + 6) < line.size()) {
                connectionInfo_.httpVersion = line.substr(pathEnd + 6, line.size() - (pathEnd + 6) - 1);
            } else {
                connectionInfo_.httpVersion = "1.1";
            }
        }

        getline(stream, line);
        size_t paramEnd;
        while ((paramEnd = line.find(':')) != std::string::npos) {
            size_t valueStart = paramEnd + 1;
            if ((valueStart) < line.size()) {
                if (line[valueStart] == ' ') {
                    valueStart++;
                }
                if (valueStart < line.size()) {
                    connectionInfo_.header.insert(
                        std::make_pair(line.substr(0, paramEnd), line.substr(valueStart, line.size() - valueStart - 1)));
                }
            }
            getline(stream, line);
        }
    }
    if (!connectionInfo_.header.count("Sec-WebSocket-Key")) {
        throw Error("Invalid WebSocket header: Missing 'Sec-WebSocket-Key'", __FILE__, __LINE__);
    }
}

void WebSocketProtocol::sendHandshakeResponse(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    auto serverKey = connectionInfo_.header.find("Sec-WebSocket-Key")->second;
    serverKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    SHA1 sha;
    uint32_t messageDigest[5];
    sha.Reset();
    sha << serverKey.c_str();
    sha.Result(messageDigest);
    // convert sha1 hash bytes to network byte order because this sha1
    // library works on ints rather than bytes
    for (int i = 0; i < 5; ++i) {
        messageDigest[i] = swap_uint32(messageDigest[i]);
    }
    serverKey = base64_encode(reinterpret_cast<const unsigned char*>(messageDigest), 20);

    std::stringstream stream;
    stream << "HTTP/1.1 101 Web Socket Protocol Handshake\r\n";
    stream << "Upgrade: websocket\r\n";
    stream << "Connection: Upgrade\r\n";
    stream << "Sec-WebSocket-Accept: " << serverKey << "\r\n";
    stream << "\r\n";
    auto responseStr = stream.str();
    connection.send(ByteArray::createFromRaw(responseStr.c_str(), responseStr.size()));
}

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

ByteArray WebSocketProtocol::readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    std::lock_guard<IStreamConnection> lock(connection);

    // read the flags byte
    auto data = readExactly(connection, 1, timeout);
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
        throw ConnectionClosedError("WebSocket connection closed", connection.identifier(), __FILE__, __LINE__);
    }

    // read the basic payload byte
    data.append(readExactly(connection, 1, timeout));

    // read additional payload-size bytes and the mask bytes
    unsigned char basicSize = data[1] & 0x7F;
    uint64_t payloadSize = 0;
    int maskOffset = 2;
    if (basicSize <= 125) {
        payloadSize = basicSize;
        data.append(readExactly(connection, 4, timeout)); // 4 bytes mask
    } else if (basicSize == 126) {
        data.append(readExactly(connection, 6, timeout)); // 2 bytes payload length + 4 bytes mask
        payloadSize = swap_uint16(*reinterpret_cast<uint16_t*>(data.data() + 2));
        maskOffset = 4;
    } else if (basicSize == 127) {
        data.append(readExactly(connection, 12, timeout)); // 8 bytes payload length + 4 bytes mask
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
    ByteArray payload = readExactly(connection, static_cast<uint32_t>(payloadSize), timeout);
    unsigned char* mask = data.data() + maskOffset;
    for (unsigned long int i = 0; i < payloadSize; ++i) {
        payload[i] ^= mask[i % 4];
    }
    return payload;
}

void WebSocketProtocol::writeFrameToStream(IStreamConnection& connection, ByteArray frame, std::chrono::milliseconds timeout) {
    auto payloadSize = frame.size();
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
    sendBuffer.append(frame);

    connection.send(std::move(sendBuffer));
}