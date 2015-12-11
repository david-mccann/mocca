//#pragma once
//
//#include "mocca/net/IProtocolConnectionAcceptor.h"
//
//namespace mocca {
//namespace net {
//template <typename PhysicalConnectionAcceptorType> class MoccaConnectionAcceptor : public IProtocolConnectionAcceptor {
//public:
//    MoccaConnectionAcceptor(std::unique_ptr<PhysicalConnectionAcceptorType> physicalConnectionAcceptor);
//    std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout) override;
//
//private:
//    std::unique_ptr<PhysicalConnectionAcceptorType> physicalConnectionAcceptor_;
//};
//}
//}