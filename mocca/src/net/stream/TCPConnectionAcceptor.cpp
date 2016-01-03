#include "mocca/net/stream/TCPConnectionAcceptor.h"

#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/stream/TCPConnection.h"

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#endif

namespace mocca {
namespace net {

TCPConnectionAcceptor::TCPConnectionAcceptor(int port)
    : port_(port) {

    try {
        server_.SetReuseAddress(true);
        server_.SetNonBlocking(true);
        server_.Bind(IVDA::NetworkAddress(IVDA::NetworkAddress::Any, port_));
        server_.Listen(3); // ???
    } catch (const IVDA::SocketException& err) {
        std::string internalError = mocca::joinString(err.what(), ", ", err.internalError());
        throw NetworkError("Network error while binding to port (internal error: " + internalError + ")", __FILE__, __LINE__);
    }
}

std::unique_ptr<IStreamConnection> TCPConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    IVDA::TCPSocket* connectionSocket = nullptr;
    try {
        server_.AcceptNewConnection((IVDA::ConnectionSocket**)&connectionSocket, static_cast<uint32_t>(timeout.count()));
    } catch (const IVDA::SocketConnectionException&) {
        // a connection terminated before it could be accepted; presumably this is not a
        // critical situation, so we ignore the error
    }
    if (connectionSocket) {
        auto socketPtr = std::unique_ptr<IVDA::ConnectionSocket>(connectionSocket);
        auto ip = connectionSocket->GetPeerAddress();
        LDEBUG("Accepted TCP connection on port " << port_ << " from " << ip);
        return std::unique_ptr<IStreamConnection>(new TCPConnection(std::move(socketPtr)));
    }
    return nullptr;
}

std::string TCPConnectionAcceptor::protocol() const {
    return "tcp";
}

#ifdef WIN32
std::string TCPConnectionAcceptor::getLastWinsockError() const {
    LPSTR s = nullptr;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&s, 0, NULL);
    std::string result(s);
    LocalFree(s);
    return result;
}

Endpoint TCPConnectionAcceptor::connectionEndpoint() const {
    char hostName[128];
    if (gethostname(hostName, sizeof(hostName))) {
        throw NetworkError("Error retrieving hostname (" + getLastWinsockError() + ")", __FILE__, __LINE__);
    }

    std::string address("127.0.0.1");
    addrinfo* infos;
    getaddrinfo(hostName, nullptr, nullptr, &infos);
    for (addrinfo* ai = infos; ai != nullptr; ai = ai->ai_next) {
        if (ai->ai_family == AF_INET) {
            sockaddr_in* addr = (struct sockaddr_in*)ai->ai_addr;
            address = inet_ntoa(addr->sin_addr);
        }
    }
    return Endpoint(protocol(), address + ":" + std::to_string(port_));
}
#else
Endpoint TCPConnectionAcceptor::connectionEndpoint() const {
    std::string address("127.0.0.1");
    ifaddrs* ifAddrStruct = nullptr;
    getifaddrs(&ifAddrStruct);
    for (ifaddrs* ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {
            in_addr* addr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, addr, addressBuffer, INET_ADDRSTRLEN);
            if (addressBuffer[0] == '1') {
                address = addressBuffer;
            }
        }
    }
    if (ifAddrStruct != nullptr) {
        freeifaddrs(ifAddrStruct);
    }
    return Endpoint(protocol(), address + ":" + std::to_string(port_));
}
#endif
}
}