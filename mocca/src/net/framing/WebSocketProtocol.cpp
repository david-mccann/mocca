/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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

std::string WebSocketProtocol::name() const {
    return "ws";
}

void WebSocketProtocol::performHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    receiveHandshake(connection, timeout);
    sendHandshakeResponse(connection);
}

void mocca::net::WebSocketProtocol::receiveHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    ByteArray headerBuffer;
    {
        std::lock_guard<std::mutex> lock(connection.receiveMutex());
        if (readUntil(connection, headerBuffer, "\r\n\r\n") == ReadStatus::Incomplete) {
            connection.putBack(headerBuffer);
            throw NetworkError("Timeout while trying to receive WebSocket handshake", __FILE__, __LINE__);
        }
    }
    std::string handshakeStr((char*)headerBuffer.data(), headerBuffer.size());
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

void WebSocketProtocol::sendHandshakeResponse(IStreamConnection& connection) {
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
    std::lock_guard<std::mutex> lock(connection.sendMutex());
    connection.send(ByteArray::createFromRaw(responseStr.c_str(), static_cast<uint32_t>(responseStr.size())));
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
    std::lock_guard<std::mutex> lock(connection.receiveMutex());

    // read the flags byte
    ByteArray prefixBuffer(14);
    readExactly(connection, prefixBuffer, 1, timeout);
    if (prefixBuffer.isEmpty()) {
        return ByteArray(); // no data read from stream
    }

#ifdef MOCCA_RUNTIME_CHECKS
    if (prefixBuffer[0] != 0x81 && prefixBuffer[0] != 0x88) { // 0x81 = final fragment, text frame; 0x88 = final fragment, close connection
        std::stringstream stream;
        stream << std::hex << static_cast<int>(prefixBuffer[0]);
        throw Error("Invalid WebSocket frame: unsupported or malformed (flag byte: " + stream.str() + ")", __FILE__, __LINE__);
    }
#endif

    if (prefixBuffer[0] == 0x88) {
        throw ConnectionClosedError("WebSocket connection closed", *connection.connectionID(), __FILE__, __LINE__);
    }

    // read the basic payload byte
    if (readExactly(connection, prefixBuffer, 1, timeout) == ReadStatus::Incomplete) {
        connection.putBack(prefixBuffer);
        return ByteArray();
    }

    // read additional payload-size bytes and the mask bytes
    unsigned char basicSize = prefixBuffer[1] & 0x7F;
    uint64_t payloadSize = 0;
    int maskOffset = 2;
    ReadStatus status;
    if (basicSize <= 125) {
        payloadSize = basicSize;
        status = readExactly(connection, prefixBuffer, 4, timeout); // 4 bytes mask
    } else if (basicSize == 126) {
        status = readExactly(connection, prefixBuffer, 6, timeout); // 2 bytes payload length + 4 bytes mask
        payloadSize = swap_uint16(*reinterpret_cast<uint16_t*>(prefixBuffer.data() + 2));
        maskOffset = 4;
    } else if (basicSize == 127) {
        status = readExactly(connection, prefixBuffer, 12, timeout); // 8 bytes payload length + 4 bytes mask
        payloadSize = swap_uint64(*reinterpret_cast<uint64_t*>(prefixBuffer.data() + 2));
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
    if (status == ReadStatus::Incomplete) {
        connection.putBack(prefixBuffer);
        return ByteArray();
    }

    // read and unmask payload data
    ByteArray payloadBuffer;
    if (readExactly(connection, payloadBuffer, static_cast<uint32_t>(payloadSize), timeout) == ReadStatus::Incomplete) {
        connection.putBack(payloadBuffer);
        connection.putBack(prefixBuffer);
        return ByteArray();
    }
    unsigned char* mask = prefixBuffer.data() + maskOffset;
    for (unsigned long int i = 0; i < payloadSize; ++i) {
        payloadBuffer[i] ^= mask[i % 4];
    }
    return payloadBuffer;
}


void WebSocketProtocol::writeFrameToStream(IStreamConnection& connection, ByteArray frame) {
    auto payloadSize = frame.size();
    ByteArray sendBuffer(payloadSize + 10); // header size is at most 10 bytes

    // create the flags byte
    unsigned char payloadFlags;
    
    //fixme bad hack for jpeg
    if(frame.size() > 7 && frame.data()[6] == 'J' && frame.data()[7] == 'F')
        payloadFlags = 0x82;
    else
        payloadFlags = 0x81;
    
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

    std::lock_guard<std::mutex> lock(connection.sendMutex());
    connection.send(std::move(sendBuffer));
}