//#pragma once
//
//#include "mocca/net/IProtocolConnectionAcceptor.h"
//
//namespace mocca {
//namespace net {
//
//template <typename PhysicalConnectionAcceptorType> class WSConnectionAcceptor : public IProtocolConnectionAcceptor {
//public:
//    WSConnectionAcceptor(std::unique_ptr<PhysicalConnectionAcceptorType> physicalConnectionAcceptor);
//    std::unique_ptr<IProtocolConnection> accept(std::chrono::milliseconds timeout) override;
//
//private:
//    std::unique_ptr<PhysicalConnectionAcceptorType> physicalConnectionAcceptor_;
//};
//}
//}