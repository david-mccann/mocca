//#pragma once
//
//#include "mocca/net/IProtocolConnection.h"
//
//namespace mocca {
//namespace net {
//
//template <typename PhysicalConnectionType> class MoccaConnection : public IProtocolConnection {
//public:
//    MoccaConnection(std::unique_ptr<PhysicalConnectionType> physicalConnection);
//
//    std::string identifier() const override;
//    void send(ByteArray message) const override;
//    ByteArray receive(std::chrono::milliseconds timeout) const override;
//
//private:
//    std::unique_ptr<PhysicalConnectionType> physicalConnection_;
//};
//}
//}
