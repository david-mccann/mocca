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

    addrinfo* infos;
    getaddrinfo(hostName, nullptr, nullptr, &infos);
    for (addrinfo* ai = infos; ai != nullptr; ai = ai->ai_next) {
        if (ai->ai_family == AF_INET) {
            sockaddr_in* addr = (struct sockaddr_in*)ai->ai_addr;
            Endpoint ep(protocol(), std::string(inet_ntoa(addr->sin_addr)) + ":" + std::to_string(port_));
          return ep;
        }
    }
    throw NetworkError("Error retrieving local IP address", __FILE__, __LINE__);
}
#else
Endpoint TCPConnectionAcceptor::connectionEndpoint() const {
    return Endpoint(protocol(), "127.0.0.1:1234");
    /*
    struct ifaddrs* ifAddrStruct = NULL;
    struct ifaddrs* ifa = NULL;
    void* tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        char const* wlanName = "en0";

        if (!ifa->ifa_addr || strcmp(wlanName, ifa->ifa_name) != 0) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
                                                   // is a valid IP4 Address
            tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

            if (addressBuffer[0] == '1')
                return addressBuffer;

        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
                                                           // is a valid IP6 Address
            tmpAddrPtr = &((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);

            if (addressBuffer[0] == '1')
                return addressBuffer;
        }
    }
    if (ifAddrStruct != NULL)
        freeifaddrs(ifAddrStruct);

    return "127.0.0.1";*/
}
#endif
}
}