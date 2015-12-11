//#pragma once
//
//#include "mocca/net/IProtocolConnection.h"
//
//#include <chrono>
//#include <memory>
//
//namespace mocca {
//namespace net {
//
//class IProtocolConnectionAcceptor {
//public:
//    virtual ~IProtocolConnectionAcceptor() {}
//
//    virtual std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
//};
//}
//}