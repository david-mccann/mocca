//#pragma once
//
//#include "mocca/net/IProtocolConnectionAcceptor.h"
//#include "mocca/net/LoopbackConnection.h"
//
//namespace mocca {
//namespace net {
//
//class LoopbackNetworkService;
//
//class LoopbackConnectionAcceptor : public IProtocolConnectionAcceptor {
//    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackConnection>>;
//
//public:
//    LoopbackConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);
//    virtual std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout) override;
//
//private:
//    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
//};
//}
//}