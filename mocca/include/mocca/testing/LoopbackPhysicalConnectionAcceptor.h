//#pragma once
//
//#include "mocca/testing/LoopbackPhysicalConnection.h"
//
//namespace mocca {
//namespace net {
//
//class LoopbackPhysicalConnectionAcceptor {
//    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackPhysicalConnection>>;
//
//public:
//    using ConnectionType = LoopbackPhysicalConnection;
//
//    LoopbackPhysicalConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);
//    std::unique_ptr<LoopbackPhysicalConnection> getConnection(std::chrono::milliseconds timeout);
//
//private:
//    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
//};
//}
//}