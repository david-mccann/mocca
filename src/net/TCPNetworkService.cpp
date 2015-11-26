#ifdef WIN32
#include <winsock2.h>
#else
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#endif

#include "mocca/base/ByteArray.h"
#include "mocca/net/Error.h"
#include "mocca/net/TCPNetworkService.h"
#include "mocca/net/TCPNetworkAddress.h"
#include "mocca/net/TCPConnection.h"
#include "mocca/net/TCPConnectionListener.h"

namespace mocca {
namespace net {

std::unique_ptr<AbstractConnection>
TCPNetworkService::connect(const std::string& connectionString) {
    TCPNetworkAddress networkAddress(connectionString);
    auto socket = std::unique_ptr<IVDA::TCPSocket>(new IVDA::TCPSocket());
    socket->SetNonBlocking(true);
    try {
        socket->Connect(IVDA::NetworkAddress(networkAddress.ip(), networkAddress.port()));
    } catch (const IVDA::SocketConnectionException& err) {
        throw ConnectFailedError("Could not connect to " + networkAddress.toString() +
             " (internal error: " + err.what() + ")", __FILE__, __LINE__);
    } catch (const IVDA::SocketException& err) {
        std::string internalError(err.what());
        throw NetworkError("Network error in connect operation (internal error: " + 
             internalError + ")", __FILE__, __LINE__);
    }
    return std::unique_ptr<TCPConnection>(
        new TCPConnection(networkAddress, std::move(socket)));
}

std::unique_ptr<IConnectionListener> TCPNetworkService::bind(const std::string& portString) {
    int port = TCPNetworkAddress::parsePort(portString);
    return std::unique_ptr<IConnectionListener>(new TCPConnectionListener(port));
}

#ifdef WIN32
std::string TCPNetworkService::getLastWinsockError() const {
    LPSTR s = nullptr;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&s, 0, NULL);
    std::string result(s);
    LocalFree(s);
    return result;
}

std::string TCPNetworkService::localIp() const {
    WSADATA WSAData;

    // Initialize winsock dll
    if (WSAStartup(MAKEWORD(1, 0), &WSAData)) {
        throw NetworkError("Error initializing Winsock (" + getLastWinsockError() + ")", __FILE__,
                           __LINE__);
    }

    // Get local host name
    char szHostName[128];
    if (gethostname(szHostName, sizeof(szHostName))) {
        WSACleanup();
        throw NetworkError("Error retrieving hostname (" + getLastWinsockError() + ")", __FILE__,
                           __LINE__);
    }

    // Get local IP addresses
    struct sockaddr_in SocketAddress;
    struct hostent* pHost = 0;
    pHost = ::gethostbyname(szHostName);
    if (!pHost) {
        WSACleanup();
        throw NetworkError("Error resolving ip address (" + getLastWinsockError() + ")", __FILE__,
                           __LINE__);
    }

    // take the first ip address from the list; not sure if this is always the right one(?)
    memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[0], pHost->h_length);
    std::string result(inet_ntoa(SocketAddress.sin_addr));

    WSACleanup();
    return result;
}
#else
std::string TCPNetworkService::localIp() const {
    struct ifaddrs* ifAddrStruct = NULL;
    struct ifaddrs* ifa = NULL;
    void* tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {

        char* wlanName = "en0";

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

    return "127.0.0.1";
}
#endif
}
}