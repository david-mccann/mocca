//#include "mocca/net/WSHandshake.h"
//
//#include "mocca/base/Endian.h"
//#include "mocca/net/Error.h"
//#include "mocca/net/WSConnection.h"
//
//#include "base64/base64.h"
//#include "sha1/sha1.h"
//
//#include <sstream>
//
//mocca::net::WSConnectionInfo mocca::net::parseWSHandshake(const std::string& handshake) {
//    WSConnectionInfo result;
//    std::stringstream stream(handshake);
//    std::string line;
//    getline(stream, line);
//    size_t methodEnd;
//    if ((methodEnd = line.find(' ')) != std::string::npos) {
//        size_t pathEnd;
//        if ((pathEnd = line.find(' ', methodEnd + 1)) != std::string::npos) {
//            result.method = line.substr(0, methodEnd);
//            result.path = line.substr(methodEnd + 1, pathEnd - methodEnd - 1);
//            if ((pathEnd + 6) < line.size()) {
//                result.httpVersion = line.substr(pathEnd + 6, line.size() - (pathEnd + 6) - 1);
//            } else {
//                result.httpVersion = "1.1";
//            }
//        }
//
//        getline(stream, line);
//        size_t paramEnd;
//        while ((paramEnd = line.find(':')) != std::string::npos) {
//            size_t valueStart = paramEnd + 1;
//            if ((valueStart) < line.size()) {
//                if (line[valueStart] == ' ') {
//                    valueStart++;
//                }
//                if (valueStart < line.size()) {
//                    result.header.insert(std::make_pair(line.substr(0, paramEnd), line.substr(valueStart, line.size() - valueStart - 1)));
//                }
//            }
//            getline(stream, line);
//        }
//    }
//    return result;
//}
//
//std::string mocca::net::createWSHandshakeResponse(const mocca::net::WSConnectionInfo& connectionInfo) {
//    if (!connectionInfo.header.count("Sec-WebSocket-Key")) {
//        throw Error("Invalid WebSocket header: Missing 'Sec-WebSocket-Key'", __FILE__, __LINE__);
//    }
//    auto serverKey = connectionInfo.header.find("Sec-WebSocket-Key")->second;
//    serverKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
//    SHA1 sha;
//    uint32_t messageDigest[5];
//    sha.Reset();
//    sha << serverKey.c_str();
//    sha.Result(messageDigest);
//    // convert sha1 hash bytes to network byte order because this sha1
//    // library works on ints rather than bytes
//    for (int i = 0; i < 5; ++i) {
//        messageDigest[i] = swap_uint32(messageDigest[i]);
//    }
//    serverKey = base64_encode(reinterpret_cast<const unsigned char*>(messageDigest), 20);
//
//    std::stringstream stream;
//    stream << "HTTP/1.1 101 Web Socket Protocol Handshake\r\n";
//    stream << "Upgrade: websocket\r\n";
//    stream << "Connection: Upgrade\r\n";
//    stream << "Sec-WebSocket-Accept: " << serverKey << "\r\n";
//    stream << "\r\n";
//    return stream.str();
//}