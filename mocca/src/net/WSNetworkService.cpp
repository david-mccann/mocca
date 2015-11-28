//#include "mocca/base/ByteArray.h"
//#include "mocca/net/Error.h"
//#include "mocca/net/WSNetworkService.h"
//#include "mocca/net/TCPNetworkAddress.h"
//#include "mocca/net/WSConnection.h"
//#include "mocca/net/WSConnectionAcceptor.h"
//
//namespace mocca {
//namespace net {
//
//std::unique_ptr<IProtocolConnection>
//WSNetworkService::connect(const std::string& connectionString) {
//    TCPNetworkAddress networkAddress(connectionString);
//    auto socket = std::unique_ptr<IVDA::TCPSocket>(new IVDA::TCPSocket());
//    socket->SetNonBlocking(true);
//    try {
//        socket->Connect(IVDA::NetworkAddress(networkAddress.ip(), networkAddress.port()));
//    } catch (const IVDA::SocketConnectionException& err) {
//        throw ConnectFailedError("Could not connect to " + networkAddress.toString() +
//             " (internal error: " + err.what() + ")", __FILE__, __LINE__);
//    } catch (const IVDA::SocketException& err) {
//        std::string internalError(err.what());
//        throw NetworkError("Network error in connect operation (internal error: " + 
//             internalError + ")", __FILE__, __LINE__);
//    }
//    return std::unique_ptr<WSConnection>(
//        new WSConnection(networkAddress, std::move(socket)));
//}
//
//std::unique_ptr<IProtocolConnectionAcceptor> WSNetworkService::bind(const std::string& portString) {
//    int port = TCPNetworkAddress::parsePort(portString);
//    return std::unique_ptr<IProtocolConnectionAcceptor>(new WSConnectionAcceptor(port));
//}
//
//}
//}