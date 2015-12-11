//#pragma once
//
//#include "mocca/net/IProtocolConnection.h"
//
//#include <unordered_map>
//
//namespace mocca {
//namespace net {
//
//struct WSConnectionInfo {
//    std::string method;
//    std::string path;
//    std::string httpVersion;
//    std::unordered_map<std::string, std::string> header;
//};
//
//template <typename PhysicalConnectionType> class WSConnection : public IProtocolConnection {
//public:
//    WSConnection(std::unique_ptr<PhysicalConnectionType> physicalConnection, const WSConnectionInfo& connectionInfo);
//
//    std::string identifier() const override;
//    void send(ByteArray message) const override;
//    ByteArray receive(std::chrono::milliseconds timeout) const override;
//
//    WSConnectionInfo connectionInfo() const;
//
//private:
//    std::unique_ptr<PhysicalConnectionType> physicalConnection_;
//    WSConnectionInfo connectionInfo_;
//};
//}
//}